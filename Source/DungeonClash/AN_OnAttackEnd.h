#pragma once

#include "CoreMinimal.h"
#include "DungeonClashCharacter.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_OnAttackEnd.generated.h"

class ADungeonClashCharacter;

UCLASS()
class DUNGEONCLASH_API UAN_OnAttackEnd : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
