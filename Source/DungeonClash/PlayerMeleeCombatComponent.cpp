#include "PlayerMeleeCombatComponent.h"
#include "DungeonClashCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"

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

	if (parentCharacter)
	{
		parentCharacter->SwordCol->OnComponentBeginOverlap.AddDynamic(this, &UPlayerMeleeCombatComponent::OnSwordOverlapBegin);
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
	bShouldDealDamage = false;
	attackComboIndex = 0;

	if (parentCharacter)
	{
		// disable sword collision
		parentCharacter->SwordCol->SetGenerateOverlapEvents(bShouldDealDamage);

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
	bShouldDealDamage = false;
	attackComboIndex = -1;

	if (parentCharacter)
	{
		// reenable movement
		parentCharacter->SetIfCanMove(true);

		// disable sword collision
		parentCharacter->SwordCol->SetGenerateOverlapEvents(bShouldDealDamage);

		// stop the attack animation
		parentCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(attackMontageBlendOutTime);
	}
}

void UPlayerMeleeCombatComponent::OnSlashBegin()
{
	bShouldDealDamage = true;

	if (parentCharacter)
	{
		// enable sword collision
		parentCharacter->SwordCol->SetGenerateOverlapEvents(bShouldDealDamage);
	}
}

void UPlayerMeleeCombatComponent::OnSlashEnd()
{
	bShouldDealDamage = false;

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
				parentCharacter->GetMesh()->GetAnimInstance()->Montage_Play(am_AttackFinish, 1.0f, EMontagePlayReturnType::MontageLength, attackFinishMontageStartTime);
			}
		}
	}

	if (parentCharacter)
	{
		// disable sword collision
		parentCharacter->SwordCol->SetGenerateOverlapEvents(bShouldDealDamage);
	}

	// stop buffering the attack
	bIsBufferingAttack = false;
}

void UPlayerMeleeCombatComponent::OnAttackEnd()
{
	EndAttack();
}

void UPlayerMeleeCombatComponent::OnSwordOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bShouldDealDamage) return;
	if (!IsValid(OtherActor)) return;
	if (OtherActor == GetOwner()) return;
	
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hit %s (%s)"), *OtherActor->GetActorNameOrLabel(), *OtherComp->GetName()));

	UHealthComponent* hitHealthComp = OtherActor->GetComponentByClass<UHealthComponent>();
	if (!IsValid(hitHealthComp)) return;
	
	hitHealthComp->TakeDamage(attackDamage);
}
