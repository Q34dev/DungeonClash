// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonClashCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DungeonClash.h"
#include "PlayerMeleeCombatComponent.h"
#include "HealthComponent.h"
#include "GameManager.h"

ADungeonClashCharacter::ADungeonClashCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// Create the sword mesh
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sword");
	SwordMesh->SetupAttachment(GetMesh(), "Sword_Socket");
	
	// Create the sword collision component
	SwordCol = CreateDefaultSubobject<UCapsuleComponent>("SwordCollision");
	SwordCol->SetupAttachment(SwordMesh);

	// Create the combat component
	CombatComp = CreateDefaultSubobject<UPlayerMeleeCombatComponent>(TEXT("PlayerMeleeCombat"));

	// Create the health component
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// Create the dodge roll timeline component
	DodgeRollTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("DodgeRollTimelineComponent"));

	// Create the dodge roll timeline curve
	DodgeRollTimelineCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("DodgeRollTimelineCurve"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ADungeonClashCharacter::BeginPlay()
{
	Super::BeginPlay();

	bWalkingDisabled = false;
	bJumpingDisabled = false;
	bLookingDisabled = false;
	bAttackingDisabled = false;

	// assign the hitbox
	HealthComp->Hitbox = GetCapsuleComponent();

	// assign the OnDeath function
	HealthComp->OnDeath.AddDynamic(this, &ADungeonClashCharacter::OnDeath);

	// set the default max walk speed
	if (IsValid(GetCharacterMovement()))
	{
		defMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}
	
	if (IsValid(DodgeRollTimelineComp) && IsValid(DodgeRollTimelineCurve))
	{
		// bind the dodge roll timeline events:

		OnDodgeRollTimelineUpdate.BindUFunction(this, FName{ TEXT("DodgeRollTimelineUpdate") });
		DodgeRollTimelineComp->AddInterpFloat(DodgeRollTimelineCurve, OnDodgeRollTimelineUpdate, NAME_None);
		
		OnDodgeRollTimelineFinished.BindUFunction(this, FName("DodgeRollTimelineFinished"));
		DodgeRollTimelineComp->SetTimelineFinishedFunc(OnDodgeRollTimelineFinished);
		
		DodgeRollTimelineComp->SetPlayRate(dodgeRollPlayRate);
		DodgeRollTimelineComp->SetTimelineLengthMode(TL_LastKeyFrame);
	}
}

void ADungeonClashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(GetCharacterMovement())) return;

	bIsGrounded = GetCharacterMovement()->IsMovingOnGround();

	if (bIsGrounded && !bWasGrounded)
	{ // when landed on the ground
		OnLand();
	}

	bWasGrounded = bIsGrounded;
}

void ADungeonClashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADungeonClashCharacter::DoJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeonClashCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADungeonClashCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeonClashCharacter::Look);
	
		// Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADungeonClashCharacter::DoAttack);

		// Dodge rolling
		EnhancedInputComponent->BindAction(DodgeRollAction, ETriggerEvent::Triggered, this, &ADungeonClashCharacter::DoDodgeRoll);
	}
	else
	{
		UE_LOG(LogDungeonClash, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADungeonClashCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ADungeonClashCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ADungeonClashCharacter::DoAttack()
{
	// can only attack if attacking is enabled
	if (bAttackingDisabled) return;

	// can only attack if is not currently dodge rolling
	if (bIsDodgeRolling) return;

	if (!IsValid(CombatComp)) return;

	CombatComp->Attack();
}

void ADungeonClashCharacter::OnDeath()
{
	AActor* GameManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	if (IsValid(GameManagerActor))
	{
		AGameManager* GameManager = Cast<AGameManager>(GameManagerActor);
		if (IsValid(GameManager))
		{
			// inform the game manager that the player has died
			GameManager->PlayerDied();
		}
	}

	// destroy the character
	Destroy();
}

void ADungeonClashCharacter::DoMove(float Right, float Forward)
{
	// can only move if is not currently dodge rolling
	if (bIsDodgeRolling) return;

	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ADungeonClashCharacter::DoLook(float Yaw, float Pitch)
{
	// can only look around with the camera if enabled
	if (bLookingDisabled) return;

	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADungeonClashCharacter::DoJump()
{
	// can only jump if enabled
	if (bJumpingDisabled) return;

	// can only jump if is not currently dodge rolling
	if (bIsDodgeRolling)
	{
		// set to buffer the input to jump later
		bIsBufferingJump = true;
		bIsBufferingDodgeRoll = false;
		return;
	}

	if (GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->IsJumpAllowed())
	{ // if is on the ground and can jump

		// play the jump sound (with randomized pitch)
		PlaySound(sb_Jump, .9f, .3f);
	}

	// signal the character to jump
	Jump();
}

void ADungeonClashCharacter::DoJumpStart()
{
	// can only jump if enabled
	if (bJumpingDisabled) return;

	// signal the character to jump
	Jump();
}

void ADungeonClashCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ADungeonClashCharacter::OnLand()
{
	// play the land sound (with randomized pitch)
	PlaySound(sb_Land, .9f, .2f);

	if (bIsBufferingDodgeRoll)
	{ // if was buffering the dodge roll when landed

		// dodge roll
		bIsBufferingDodgeRoll = false;
		DoDodgeRoll();
	}
}

FOnDamageReceived& ADungeonClashCharacter::GetOnDamageReceivedEvent() const
{
	FOnDamageReceived NullReturn;
	if (!IsValid(HealthComp)) return NullReturn;

	return HealthComp->OnDamageReceived;
}

void ADungeonClashCharacter::PlaySound(USoundBase* SB, float volumeMultiplier, float pitchRandomizeRange)
{
	if (!IsValid(SB)) return;
	if (!IsValid(GetWorld())) return;

	float pitch = (pitchRandomizeRange > .0f) ? FMath::RandRange(1.f - pitchRandomizeRange, 1.f + pitchRandomizeRange) : 1.f;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB, GetActorLocation(), volumeMultiplier, pitch);
}

void ADungeonClashCharacter::UpdateMovementInput()
{
	if (GetController() == nullptr) return;
	
	// find out which way is forward
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// update movement input direction
	MovementInputDir = FVector::Zero();
	MovementInputDir += RightDirection * MovementVector.X;
	MovementInputDir += ForwardDirection * MovementVector.Y;
}

void ADungeonClashCharacter::SetIfCanMove(bool canWalk, bool canJump, bool canDodgeRoll)
{
	if (GetController() == nullptr) return;

	bWalkingDisabled = !canWalk;
	bJumpingDisabled = !canJump;
	bDodgeRollingDisabled = !canDodgeRoll;

	if (canWalk)
	{
		// enable walking (dont ignore movement input)
		GetController()->SetIgnoreMoveInput(false);
	}
	else
	{
		// disable walking (ignore movement input)
		GetController()->SetIgnoreMoveInput(true);
	}
}

void ADungeonClashCharacter::SetIfCanLook(bool canLook)
{
	bLookingDisabled = !canLook;
}

void ADungeonClashCharacter::SetIfCanAttack(bool canAttack)
{
	bAttackingDisabled = !canAttack;
}

void ADungeonClashCharacter::SetMaxWalkSpeed(float val)
{
	if (IsValid(GetCharacterMovement()))
	{
		GetCharacterMovement()->MaxWalkSpeed = val;

		if (val <= 0.f)
		{
			// disable walking
			bWalkingDisabled = true;
			if (IsValid(GetController()))
				GetController()->SetIgnoreMoveInput(true);
		}
	}
}

void ADungeonClashCharacter::ResetMaxWalkSpeed()
{
	if (IsValid(GetCharacterMovement()))
	{
		GetCharacterMovement()->MaxWalkSpeed = defMaxWalkSpeed;
	}
}

void ADungeonClashCharacter::DoDodgeRoll()
{
	// can only dodge roll if enabled
	if (bDodgeRollingDisabled) return;

	// can only dodge roll if is not currently dodge rolling and is on the ground
	if (bIsDodgeRolling || !GetCharacterMovement()->IsMovingOnGround())
	{
		if (bCanBufferDodgeRoll)
		{
			// set to buffer the input to dodge roll later
			bIsBufferingDodgeRoll = true;
			bIsBufferingJump = false;
		}
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;

	if (IsValid(CombatComp) && CombatComp->bIsAttacking)
	{ // if is attacking
		// interrupt the attack
		CombatComp->EndAttack();
	}

	// start dodge rolling
	bIsDodgeRolling = true;
	bCanBufferDodgeRoll = false;

	// make the player invincible during the dodge roll
	if (IsValid(HealthComp)) HealthComp->SetIfInvincible(true);
	
	UpdateMovementInput();
	if (!MovementInputDir.IsZero())
	{
		// rotate immediately towards the input direction
		FRotator fRot = MovementInputDir.Rotation();
		SetActorRotation(fRot, ETeleportType::TeleportPhysics);
	}

	// play the sound
	PlaySound(sb_DodgeRoll, .9f, .3f);

	// play the animation montage
	AnimInstance->Montage_Play(am_DodgeRoll, dodgeRollPlayRate, EMontagePlayReturnType::MontageLength, dodgeRollMontageStartTime);

	// play the timeline
	if (IsValid(DodgeRollTimelineComp))
	{
		DodgeRollTimelineComp->SetTimelineLength(dodgeRollForce);
		DodgeRollTimelineComp->PlayFromStart();
	}
}

void ADungeonClashCharacter::OnDodgeRollEnd()
{
	// enable buffering the dodge roll input
	bCanBufferDodgeRoll = true;
}

void ADungeonClashCharacter::DodgeRollTimelineUpdate(float curveVal)
{
	if (!IsValid(DodgeRollTimelineComp) || !IsValid(DodgeRollTimelineCurve)) return;

	if (DodgeRollTimelineComp->IsPlaying())
	{ // if the timeline is playing

		if (IsValid(GetCharacterMovement()))
		{
			// move forward with the force from the timeline curve
			FVector vec = curveVal * dodgeRollForce * GetActorForwardVector();
			GetCharacterMovement()->AddInputVector(vec);
		}
	}
}

void ADungeonClashCharacter::DodgeRollTimelineFinished()
{
	// stop dodge rolling
	bIsDodgeRolling = false;

	// disable invincibility
	if (IsValid(HealthComp)) HealthComp->SetIfInvincible(false);

	if (bIsBufferingDodgeRoll)
	{ // if was buffering the dodge roll when ended dodge rolling

		// dodge roll again
		bIsBufferingDodgeRoll = false;
		DoDodgeRoll();
	}
	else if (bIsBufferingJump)
	{ // if was buffering the jump when ended dodge rolling
		
		// jump
		bIsBufferingJump = false;
		DoJump();
	}
}
