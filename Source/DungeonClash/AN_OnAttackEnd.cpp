#include "AN_OnAttackEnd.h"

void UAN_OnAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ADungeonClashCharacter* OwnerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter) return;

	UPlayerMeleeCombatComponent* CombatComponent = OwnerCharacter->GetComponentByClass<UPlayerMeleeCombatComponent>();
	if (!CombatComponent) return;

	CombatComponent->OnAttackEnd();
}
