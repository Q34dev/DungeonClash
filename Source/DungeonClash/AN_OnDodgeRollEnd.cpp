#include "AN_OnDodgeRollEnd.h"
#include "DungeonClashCharacter.h"

void UAN_OnDodgeRollEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ADungeonClashCharacter* OwnerCharacter = Cast<ADungeonClashCharacter>(MeshComp->GetOwner());
	if (!IsValid(OwnerCharacter)) return;

	// notify the owner character that the dodge roll has ended
	OwnerCharacter->OnDodgeRollEnd();
}
