#include "ANS_Slash.h"
#include "DungeonClashCharacter.h"
#include "EnemyCharacter.h"

void UANS_Slash::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ADungeonClashCharacter* OwnerPlayerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (OwnerPlayerCharacter)
	{ // owner is the player

		UPlayerMeleeCombatComponent* CombatComponent = OwnerPlayerCharacter->GetComponentByClass<UPlayerMeleeCombatComponent>();
		if (CombatComponent) CombatComponent->OnSlashBegin();
	}
	else
	{
		AEnemyCharacter* OwnerEnemyCharacter = Cast<AEnemyCharacter>(MeshComp->GetOwner());
		if (OwnerEnemyCharacter)
		{ // owner is an enemy

			OwnerEnemyCharacter->OnSlashBegin();
		}
	}
}

void UANS_Slash::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ADungeonClashCharacter* OwnerPlayerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (OwnerPlayerCharacter)
	{ // owner is the player

		UPlayerMeleeCombatComponent* CombatComponent = OwnerPlayerCharacter->GetComponentByClass<UPlayerMeleeCombatComponent>();
		if (CombatComponent) CombatComponent->OnSlashEnd();
	}
	else
	{
		AEnemyCharacter* OwnerEnemyCharacter = Cast<AEnemyCharacter>(MeshComp->GetOwner());
		if (OwnerEnemyCharacter)
		{ // owner is an enemy

			OwnerEnemyCharacter->OnSlashEnd();
		}
	}
}
