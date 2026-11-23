#include "AN_OnWalkStep.h"
#include "DungeonClashCharacter.h"
#include "EnemyCharacter.h"

void UAN_OnWalkStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ADungeonClashCharacter* OwnerPlayerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (IsValid(OwnerPlayerCharacter))
	{ // owner is the player

		OwnerPlayerCharacter->PlaySound(sb_WalkStepSound, .2f, .2f);
	}
	else
	{
		AEnemyCharacter* OwnerEnemyCharacter = Cast<AEnemyCharacter>(MeshComp->GetOwner());
		if (IsValid(OwnerEnemyCharacter))
		{ // owner is an enemy

			OwnerEnemyCharacter->PlaySound(sb_WalkStepSound, .2f, .2f);
		}
	}
}
