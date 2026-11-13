#include "ANS_Healing.h"
#include "DungeonClashCharacter.h"

void UANS_Healing::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ADungeonClashCharacter* OwnerPlayerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (IsValid(OwnerPlayerCharacter))
		OwnerPlayerCharacter->HealAnimAddHealth();
}

void UANS_Healing::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ADungeonClashCharacter* OwnerPlayerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (IsValid(OwnerPlayerCharacter))
		OwnerPlayerCharacter->OnHealAnimEnd();
}
