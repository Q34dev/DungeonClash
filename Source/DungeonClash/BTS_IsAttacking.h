#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_IsAttacking.generated.h"

UCLASS()
class DUNGEONCLASH_API UBTS_IsAttacking : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	// Constructor
	UBTS_IsAttacking();

	// Method for when the service is called
	void OnBecomeRelevant(UBehaviorTreeComponent& a_pBehaviorTreeComp, uint8* a_pNodeMem);
};
