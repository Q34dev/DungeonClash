#include "ANS_Slash.h"

void UANS_Slash::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ADungeonClashCharacter* OwnerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter) return;

	UPlayerMeleeCombatComponent* CombatComponent = OwnerCharacter->GetComponentByClass<UPlayerMeleeCombatComponent>();
	if (!CombatComponent) return;

	CombatComponent->OnSlashBegin();
}

void UANS_Slash::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ADungeonClashCharacter* OwnerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter) return;

	UPlayerMeleeCombatComponent* CombatComponent = OwnerCharacter->GetComponentByClass<UPlayerMeleeCombatComponent>();
	if (!CombatComponent) return;

	CombatComponent->OnSlashEnd();
}
