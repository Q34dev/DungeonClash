#include "PlayerMeleeCombatComponent.h"
#include "DungeonClashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UPlayerMeleeCombatComponent::UPlayerMeleeCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPlayerMeleeCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		// assign the parent character
		parentCharacter = Cast<ADungeonClashCharacter>(GetOwner());
	}
}

// Called every frame
void UPlayerMeleeCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerMeleeCombatComponent::Attack()
{
	// can only attack if is on the ground
	if (parentCharacter && !parentCharacter->GetCharacterMovement()->IsMovingOnGround()) return;

	if (!bIsAttacking)
	{ // if is not attacking

		StartAttack();
	}
	else if (!bIsBufferingAttack)
	{ // if is attacking (but not buffered yet)

		// set to buffer the attack to continue it after the slash ends
		bIsBufferingAttack = true;
	}
}

void UPlayerMeleeCombatComponent::StartAttack()
{
	bIsAttacking = true;
	attackComboIndex = 0;

	if (parentCharacter)
	{
		// play the attack animation
		parentCharacter->GetMesh()->GetAnimInstance()->Montage_Play(am_AttackCombo);

		// disable movement during the attack
		parentCharacter->SetIfCanMove(false);
	}
}

void UPlayerMeleeCombatComponent::EndAttack()
{
	bIsAttacking = false;
	bIsBufferingAttack = false;
	attackComboIndex = -1;

	if (parentCharacter)
	{
		// reenable movement
		parentCharacter->SetIfCanMove(true);

		// stop the attack animation
		parentCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(attackMontageBlendOutTime);
	}
}

void UPlayerMeleeCombatComponent::OnSlashBegin()
{

}

void UPlayerMeleeCombatComponent::OnSlashEnd()
{
	if (!bIsBufferingAttack)
	{ // if attack not buffered

		if (attackComboIndex == 0)
		{ // after the first slash (in the middle of the combo animation)

			// stop the attack
			EndAttack();

			// stop the attack animation
			if (parentCharacter) parentCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(attackMontageBlendOutTime);
		}
	}
	else
	{ // if attack buffered (player wants to continue the attack combo)

		// next combo index
		attackComboIndex++;

		if (attackComboIndex == 2)
		{ // combo finishing slash

			if (parentCharacter)
			{
				// stop the attack animation
				parentCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(attackMontageBlendOutTime);

				// play the combo finish animation
				parentCharacter->GetMesh()->GetAnimInstance()->Montage_Play(am_AttackFinish);
			}
		}
	}

	// stop buffering the attack
	bIsBufferingAttack = false;
}

void UPlayerMeleeCombatComponent::OnAttackEnd()
{
	EndAttack();
}
