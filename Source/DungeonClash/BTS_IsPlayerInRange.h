#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_IsPlayerInRange.generated.h"

UCLASS()
class DUNGEONCLASH_API UBTS_IsPlayerInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	// Constructor
	UBTS_IsPlayerInRange();

	// Method for when the service is called
	void OnBecomeRelevant(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float range = 100.f;
};
