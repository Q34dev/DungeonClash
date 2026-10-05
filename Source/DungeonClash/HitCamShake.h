#pragma once

#include "CoreMinimal.h"
#include "Shakes/LegacyCameraShake.h"
#include "HitCamShake.generated.h"

UCLASS()
class DUNGEONCLASH_API UHitCamShake : public ULegacyCameraShake
{
	GENERATED_BODY()
	
public:
	UHitCamShake();
};
