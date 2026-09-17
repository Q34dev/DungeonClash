#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ChasePlayer.generated.h"

UCLASS()
class DUNGEONCLASH_API UBTT_ChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_ChasePlayer(FObjectInitializer const& a_pObjectInit);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& a_pBehaviorTreeComp, uint8* a_pNodeMem);
};
