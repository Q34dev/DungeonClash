#include "EnemyCharacter.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HealthBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyManager.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BrainComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the health component
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("EnemyHealthComponent"));

	// Create the health bar widget component
	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthBar"));
	HealthBarWidgetComp->SetupAttachment(GetMesh());

	// Create the attack collision component
	AttackCol = CreateDefaultSubobject<USphereComponent>("AttackCollision");
	AttackCol->SetupAttachment(GetMesh(), "Attack_Socket");
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// assign the hitbox
	HealthComp->Hitbox = GetCapsuleComponent();

	// assign the Health Component functions
	HealthComp->OnHealthUpdate.AddDynamic(this, &AEnemyCharacter::OnHealthUpdate);
	HealthComp->OnDeath.AddDynamic(this, &AEnemyCharacter::OnDeath);

	if (IsValid(HealthBarWidgetComp))
	{
		// get the health bar class
		if (IsValid(HealthBarWidgetComp->GetWidget()))
			HealthBar = Cast<UHealthBar>(HealthBarWidgetComp->GetWidget());

		// hide the health bar at the start
		HealthBarWidgetComp->SetVisibility(false);
	}

	// bind the attack collision overlap method
	AttackCol->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnAttackOverlapBegin);
	
	// clear the hit actors array
	HitActors.Empty();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::Attack()
{
	if (!IsValid(GetMesh()) || !IsValid(GetMesh()->GetAnimInstance())) return;
	
	// stop moving
	GetCharacterMovement()->StopMovementImmediately();

	bShouldDealDamage = false;

	// disable the attack collision
	if (IsValid(AttackCol)) AttackCol->SetGenerateOverlapEvents(bShouldDealDamage);

	// play the attack animation
	if (IsValid(am_Attack)) GetMesh()->GetAnimInstance()->Montage_Play(am_Attack);
}

void AEnemyCharacter::OnSlashBegin()
{
	bShouldDealDamage = true;

	// clear the hit actors array
	HitActors.Empty();

	// enable the attack collision
	if (IsValid(AttackCol)) AttackCol->SetGenerateOverlapEvents(bShouldDealDamage);

	// play the attack sound
	if (IsValid(sb_Attack) && IsValid(GetWorld()))
	{
		PlaySound(sb_Attack, .8f, .2f);
	}
}

void AEnemyCharacter::OnSlashEnd()
{
	bShouldDealDamage = false;

	// disable the attack collision
	if (IsValid(AttackCol)) AttackCol->SetGenerateOverlapEvents(bShouldDealDamage);

	// clear the hit actors array
	HitActors.Empty();
}

void AEnemyCharacter::Hit(UHealthComponent* HitActor)
{
	// skip if this actor was already hit during the current attack
	if (HitActors.Contains(HitActor)) return;

	// deal damage to the hit actor
	HitActor->TakeDamage(attackDamage);

	// add the hit actor to the array
	HitActors.Add(HitActor);
}

void AEnemyCharacter::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bShouldDealDamage) return;
	if (!IsValid(OtherActor)) return;
	if (OtherActor == this) return;
	
	// check if the hit actor has a health component
	UHealthComponent* HitHealthComp = OtherActor->GetComponentByClass<UHealthComponent>();
	if (!IsValid(HitHealthComp)) return;

	// check if the hit collision component was the hitbox of the hit actor
	if (HitHealthComp->Hitbox != OtherComp) return;

	Hit(HitHealthComp);
}

void AEnemyCharacter::ActivateEnemy()
{
	// reset the health value
	HealthComp->ResetHealth();

	// show the actor
	SetActorHiddenInGame(false);

	// enable collision
	SetActorEnableCollision(true);

	// enable ticking
	SetActorTickEnabled(true);

	// reset force
	GetCharacterMovement()->ClearAccumulatedForces();
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	LaunchCharacter(FVector::ZeroVector, true, true);

	// enable AI logic
	AAIController* AIController = Cast<AAIController>(GetController());
	if (IsValid(AIController))
	{
		UBrainComponent* BrainComp = AIController->GetBrainComponent();
		if (IsValid(BrainComp))
		{
			BrainComp->StartLogic();
		}
	}

	// hide the health bar
	HealthBarWidgetComp->SetVisibility(false);
	
	// reset the health bar
	if (IsValid(HealthBar)) HealthBar->ResetHealthBar();
}

void AEnemyCharacter::DeactivateEnemy()
{
	// hide the actor
	SetActorHiddenInGame(true);
	
	// disable collision
	SetActorEnableCollision(false);

	// disable ticking
	SetActorTickEnabled(false);

	// reset force
	GetCharacterMovement()->ClearAccumulatedForces();
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	LaunchCharacter(FVector::ZeroVector, true, true);

	// disable AI logic
	AAIController* AIController = Cast<AAIController>(GetController());
	if (IsValid(AIController))
	{
		UBrainComponent* BrainComp = AIController->GetBrainComponent();
		if (IsValid(BrainComp))
		{
			BrainComp->StopLogic(FString("Enemy deactivated"));
		}
	}

	// reset the animation
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);

	// hide the health bar
	HealthBarWidgetComp->SetVisibility(false);
}

void AEnemyCharacter::PlaySound(USoundBase* SB, float volumeMultiplier, float pitchRandomizeRange)
{
	if (!IsValid(SB)) return;
	if (!IsValid(GetWorld())) return;

	float pitch = (pitchRandomizeRange > .0f) ? FMath::RandRange(1.f - pitchRandomizeRange, 1.f + pitchRandomizeRange) : 1.f;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB, GetActorLocation(), volumeMultiplier, pitch);
}

void AEnemyCharacter::OnHealthUpdate(float newHealth, float previousHealth, float maxHealth)
{
	if (newHealth < previousHealth)
	{ // enemy was damaged

		// show the health bar
		HealthBarWidgetComp->SetVisibility(true);

		// update the health bar
		if (IsValid(HealthBar)) HealthBar->OnHealthChanged(newHealth, previousHealth, maxHealth);
	}
}

void AEnemyCharacter::OnDeath()
{
	// deactivate the enemy
	DeactivateEnemy();

	// get the enemy manager
	AActor* EnemyManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass());
	if (IsValid(EnemyManagerActor))
	{
		AEnemyManager* EnemyManager = Cast<AEnemyManager>(EnemyManagerActor);
		if (IsValid(EnemyManager))
		{
			// inform the enemy manager of the death
			EnemyManager->OnEnemyDied(this);
		}
	}
}
