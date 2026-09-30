#include "AN_OnWalkStep.h"
#include "DungeonClashCharacter.h"

void UAN_OnWalkStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ADungeonClashCharacter* OwnerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (!IsValid(OwnerCharacter)) return;

	OwnerCharacter->PlaySound(sb_WalkStepSound, true, .5f);
}
