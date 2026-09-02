#pragma once

#include "CoreMinimal.h"
#include "DungeonClashCharacter.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_Slash.generated.h"

class ADungeonClashCharacter;

UCLASS()
class DUNGEONCLASH_API UANS_Slash : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
