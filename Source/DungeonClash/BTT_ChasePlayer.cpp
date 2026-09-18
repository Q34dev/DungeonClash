#include "BTT_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "EnemyKeys.h"
#include "Kismet/GameplayStatics.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& a_pBehaviorTreeComp, uint8* a_pNodeMem)
{
	// get AI controller
	auto const pAIController = Cast<AEnemyAIController>(a_pBehaviorTreeComp.GetAIOwner());

	if (pAIController->GetBlackboardComp()->GetValueAsBool(EnemyKeys::isPlayerInRange)
	 || pAIController->GetBlackboardComp()->GetValueAsBool(EnemyKeys::isAttacking))
	{ // if the player is in range or enemy is currently attacking

		// don't move towards the player
		// finish task execution
		FinishLatentTask(a_pBehaviorTreeComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// get navigation system
	UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (pNavSystem)
	{
		// get player location (the set target location)
		FVector pLocation = pAIController->GetBlackboardComp()->GetValueAsVector(EnemyKeys::targetLocation);

		// move towards the player
		pAIController->MoveToLocation(pLocation);
	}

	// finish task execution
	FinishLatentTask(a_pBehaviorTreeComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
