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

	if (AttackMontageFinished(pEnemy))
	{ // if the attack montage is finished

		// check if can attack (can attack if player is in range)
		bool bCanAttack = pAIController->GetBlackboardComp()->GetValueAsBool(EnemyKeys::isPlayerInRange);
		if (bCanAttack)
		{
			// run the enemy attack method
			pEnemy->Attack();
		}
	}

	// finish execution
	FinishLatentTask(a_pBehaviorTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

bool UBTT_Attack::AttackMontageFinished(AEnemyCharacter* a_pEnemy)
{
	UAnimInstance* pAnimInstance = a_pEnemy->GetMesh()->GetAnimInstance();
	if (!IsValid(pAnimInstance)) return false;

	return pAnimInstance->Montage_GetIsStopped(a_pEnemy->am_Attack);
}
