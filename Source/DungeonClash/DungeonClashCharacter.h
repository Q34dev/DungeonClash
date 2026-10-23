// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PlayerMeleeCombatComponent.h"
#include "HealthComponent.h"
#include "Components/TimelineComponent.h"
#include "DungeonClashCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthPotionCountChanged);

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class USoundBase;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ADungeonClashCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
public:

	// Sword mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SwordMesh;

	// Sword collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* SwordCol;

	// Player Melee Combat component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPlayerMeleeCombatComponent* CombatComp;

	// Health component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UHealthComponent* HealthComp;

	// Dodge roll animation montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll")
	UAnimMontage* am_DodgeRoll;

	// Dodge roll animation play rate scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll")
	float dodgeRollPlayRate = 1.f;

	// Dodge roll animation montage start time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll")
	float dodgeRollMontageStartTime = 0.f;

	// Dodge roll move force
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll")
	float dodgeRollForce = 1.f;

	// Dodge roll timeline component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll")
	UTimelineComponent* DodgeRollTimelineComp;

	// Dodge roll timeline curve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll")
	UCurveFloat* DodgeRollTimelineCurve;

	// The amount of health added when used a health potion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float healedHealthAmount = 1.f;

	// A boolean indicating if walking input is ignored
	UPROPERTY(BlueprintReadOnly)
	bool bWalkingDisabled = false;
	// A boolean indicating if jumping input is ignored
	UPROPERTY(BlueprintReadOnly)
	bool bJumpingDisabled = false;
	// A boolean indicating if camera looking input is ignored
	UPROPERTY(BlueprintReadOnly)
	bool bLookingDisabled = false;
	// A boolean indicating if attacking input is ignored
	UPROPERTY(BlueprintReadOnly)
	bool bAttackingDisabled = false;
	// A boolean indicating if dodge rolling input is ignored
	UPROPERTY(BlueprintReadOnly)
	bool bDodgeRollingDisabled = false;

	// Booleans indicating if the player is/was on the ground
	UPROPERTY(BlueprintReadOnly)
	bool bIsGrounded = false;
	UPROPERTY(BlueprintReadOnly)
	bool bWasGrounded = true;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	/** Dodge roll Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeRollAction;

	/** Heal Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* HealAction;

	// Jump sound
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* sb_Jump;

	// Land sound
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* sb_Land;

	// Dodge roll sound
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* sb_DodgeRoll;

public:

	/** Constructor */
	ADungeonClashCharacter();	

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for attack input */
	void DoAttack();

	// character death method
	UFUNCTION()
	void OnDeath();

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs and takes the movementDisabled boolean into consideration */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJump();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	// Called when the player lands on the ground
	UFUNCTION()
	void OnLand();

	// Handling the dodge roll
	bool bIsDodgeRolling = false;
	void DoDodgeRoll();
	void OnDodgeRollEnd(); // Anim Notify
	FOnTimelineFloat OnDodgeRollTimelineUpdate{};
	FOnTimelineEvent OnDodgeRollTimelineFinished;
	UFUNCTION()
	void DodgeRollTimelineUpdate(float curveVal);
	UFUNCTION()
	void DodgeRollTimelineFinished();

	// Healing using a health potion
	void DoHeal();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FOnHealthChanged& GetOnHealthChangedEvent() const;

	void PlaySound(USoundBase* SB, float volumeMultiplier = 1.f, float pitchRandomizeRange = .0f);

	void UpdateMovementInput();

	// A method to enable/disable receiving movement input
	UFUNCTION()
	void SetIfCanMove(bool canWalk, bool canJump, bool canDodgeRoll);

	// A method to enable/disable receiving camera looking input
	UFUNCTION()
	void SetIfCanLook(bool canLook);
	
	// A method to enable/disable receiving attack input
	UFUNCTION()
	void SetIfCanAttack(bool canAttack);

	// A method to set the character max walk speed value
	UFUNCTION()
	void SetMaxWalkSpeed(float val);

	// A method to reset max walk speed to the default value
	UFUNCTION()
	void ResetMaxWalkSpeed();

	// Executed when the player picks up a health potion item
	UFUNCTION()
	void PickedUpAHealthPotion();

	// Managing the health potion count
	void SetHealthPotionCount(int count);
	int GetHealthPotionCount();

	UPROPERTY(BlueprintAssignable)
	FOnHealthPotionCountChanged OnHealthPotionCountChanged;

protected:

	// movement input vector
	FVector2D MovementVector;
	FVector MovementInputDir;

	// the default max walk speed value
	float defMaxWalkSpeed = 100.f;

	// input buffers
	bool bIsBufferingJump = false;
	bool bIsBufferingDodgeRoll = false;
	bool bCanBufferDodgeRoll = false;

	// the amount of health potions the player has
	int healthPotionCount = 0;
};
