#include "BTT_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "EnemyKeys.h"
#include "Kismet/GameplayStatics.h"

UBTT_FindPlayerLocation::UBTT_FindPlayerLocation(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTT_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& a_pBehaviorTreeComp, uint8* a_pNodeMem)
{
	// get AI controller
	auto const pAIController = Cast<AEnemyAIController>(a_pBehaviorTreeComp.GetAIOwner());

	// get navigation system
	UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (IsValid(pNavSystem))
	{
		// get player controller reference
		APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(pPlayerController))
		{
			APawn* pPlayer = pPlayerController->GetPawn();
			if (IsValid(pPlayer))
			{
				// set the target location to player location
				pAIController->GetBlackboardComp()->SetValueAsVector(EnemyKeys::targetLocation, pPlayer->GetActorLocation());
			}
		}
	}

	// finish task execution
	FinishLatentTask(a_pBehaviorTreeComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
