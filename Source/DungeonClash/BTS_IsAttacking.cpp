#include "BTS_IsAttacking.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include "EnemyKeys.h"

UBTS_IsAttacking::UBTS_IsAttacking()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Attacking");
}

void UBTS_IsAttacking::OnBecomeRelevant(UBehaviorTreeComponent& a_pBehaviorTreeComp, uint8* a_pNodeMem)
{
	Super::OnBecomeRelevant(a_pBehaviorTreeComp, a_pNodeMem);

	// get enemy AI controller
	auto const pAIController = Cast<AEnemyAIController>(a_pBehaviorTreeComp.GetAIOwner());

	// get enemy character
	AEnemyCharacter* pEnemy = Cast<AEnemyCharacter>(pAIController->GetPawn());

	// get enemy animation instance
	UAnimInstance* pAnimInstance = pEnemy->GetMesh()->GetAnimInstance();
	if (!IsValid(pAnimInstance)) return;

	// check if is playing the attack animation
	bool bIsAttacking = pAnimInstance->Montage_IsPlaying(pEnemy->am_Attack);

	// set the blackboard key value
	pAIController->GetBlackboardComp()->SetValueAsBool(EnemyKeys::isAttacking, bIsAttacking);
}
