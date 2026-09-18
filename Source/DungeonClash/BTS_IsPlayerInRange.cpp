#include "BTS_IsPlayerInRange.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyKeys.h"

UBTS_IsPlayerInRange::UBTS_IsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Range");
}

void UBTS_IsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	Super::OnBecomeRelevant(a_pTreeComp, a_pNodeMem);

	// get enemy AI controller
	AEnemyAIController* pAIController = Cast<AEnemyAIController>(a_pTreeComp.GetAIOwner());
	
	// get enemy pawn
	APawn* pEnemy = pAIController->GetPawn();

	// get player controller
	APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// set the blackboard key value
	bool bIsPlayerInRange = pEnemy->GetDistanceTo(pPlayerController->GetPawn()) <= range;
	pAIController->GetBlackboardComp()->SetValueAsBool(EnemyKeys::isPlayerInRange, bIsPlayerInRange);
}
