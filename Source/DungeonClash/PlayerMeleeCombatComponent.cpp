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

	if (parentCharacter)
	{
		// play the attack animation
		parentCharacter->GetMesh()->GetAnimInstance()->Montage_Play(am_Attack);

		// disable movement during the attack
		parentCharacter->SetIfCanMove(false);
	}
}

void UPlayerMeleeCombatComponent::EndAttack()
{
	bIsAttacking = false;
	bIsBufferingAttack = false;

	// reenable movement
	if (parentCharacter) parentCharacter->SetIfCanMove(true);
}

void UPlayerMeleeCombatComponent::OnSlashBegin()
{

}

void UPlayerMeleeCombatComponent::OnSlashEnd()
{
	if (!bIsBufferingAttack)
	{ // if attack not buffered

		// stop the attack
		EndAttack();

		// stop the attack animation
		if (parentCharacter) parentCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(.2f);
	}
}

void UPlayerMeleeCombatComponent::OnAttackEnd()
{
	EndAttack();
}
