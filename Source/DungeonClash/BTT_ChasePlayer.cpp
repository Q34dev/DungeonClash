#include "BTT_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "EnemyKeys.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& a_pBehaviorTreeComp, uint8* a_pNodeMem)
{
	// get AI controller
	auto const pAIController = Cast<AEnemyAIController>(a_pBehaviorTreeComp.GetAIOwner());

	bool isPlayerInRange = pAIController->GetBlackboardComp()->GetValueAsBool(EnemyKeys::isPlayerInRange);
	bool isAttacking = pAIController->GetBlackboardComp()->GetValueAsBool(EnemyKeys::isAttacking);

	if (isAttacking)
	{ // during an attack
		// don't move towards the player
		// finish task execution
		FinishLatentTask(a_pBehaviorTreeComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// get player location (the set target location)
	FVector targetLocation = pAIController->GetBlackboardComp()->GetValueAsVector(EnemyKeys::targetLocation);

	if (isPlayerInRange)
	{ // if the player is in range

		APawn* enemyPawn = pAIController->GetPawn();
		if (IsValid(enemyPawn))
		{
			// rotate towards the player
			FVector enemyLocation = enemyPawn->GetActorLocation();
			FRotator fRot = UKismetMathLibrary::FindLookAtRotation(enemyLocation, targetLocation);
			enemyPawn->SetActorRotation(fRot);
		}

		// don't move towards the player
		// finish task execution
		FinishLatentTask(a_pBehaviorTreeComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// get navigation system
	UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (pNavSystem)
	{
		// move towards the player
		pAIController->MoveToLocation(targetLocation);
	}

	// finish task execution
	FinishLatentTask(a_pBehaviorTreeComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
