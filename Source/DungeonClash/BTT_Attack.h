#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyCharacter.h"
#include "BTT_Attack.generated.h"

UCLASS()
class DUNGEONCLASH_API UBTT_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Attack(FObjectInitializer const& a_pObjectInit);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& a_pBehaviorTreeComp, uint8* a_pNodeMem);

	bool AttackMontageFinished(AEnemyCharacter* a_pEnemy);
};
