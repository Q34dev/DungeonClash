#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "WinStateTriggerBox.generated.h"

UCLASS()
class DUNGEONCLASH_API AWinStateTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AWinStateTriggerBox();

	UFUNCTION()
	void OnBoxOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

};
