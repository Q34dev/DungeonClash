#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& a_ppObjectInit)
{
	// get the behaviour tree from the reference
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> treeFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/Enemies/BT_Enemy.BT_Enemy'"));
	if (treeFinder.Succeeded())
	{
		m_pBehaviorTree = treeFinder.Object;
	}

	// create the behavior tree component
	m_pBehaviorTreeComp = a_ppObjectInit.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComp"));

	// create the blackboard component
	m_pBlackboardComp = a_ppObjectInit.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// run the behavior tree
	RunBehaviorTree(m_pBehaviorTree);

	// start the behavior tree
	m_pBehaviorTreeComp->StartTree(*m_pBehaviorTree);
}

void AEnemyAIController::OnPossess(APawn* a_pPawn)
{
	Super::OnPossess(a_pPawn);

	// initialize the blackboard component
	if (m_pBlackboardComp)
	{
		m_pBlackboardComp->InitializeBlackboard(*m_pBehaviorTree->BlackboardAsset);
	}
}

UBlackboardComponent* AEnemyAIController::GetBlackboardComp()
{
	return m_pBlackboardComp;
}
