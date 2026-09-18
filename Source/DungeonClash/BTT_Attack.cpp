#include "BTT_Attack.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyKeys.h"

UBTT_Attack::UBTT_Attack(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& a_pBehaviorTreeComp, uint8* a_pNodeMem)
{
	// get enemy AI controller
	auto const pAIController = Cast<AEnemyAIController>(a_pBehaviorTreeComp.GetAIOwner());

	// get enemy character
	AEnemyCharacter* pEnemy = Cast<AEnemyCharacter>(pAIController->GetPawn());

	if (!pAIController->GetBlackboardComp()->GetValueAsBool(EnemyKeys::isAttacking))
	{ // if the enemy is not attacking yet

		// check if player is in range
		bool bPlayerInRange = pAIController->GetBlackboardComp()->GetValueAsBool(EnemyKeys::isPlayerInRange);

		if (bPlayerInRange)
		{ // if player is in range

			// run the enemy attack method
			pEnemy->Attack();
		}
	}

	// finish task execution
	FinishLatentTask(a_pBehaviorTreeComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
