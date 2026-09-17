#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class DUNGEONCLASH_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController(FObjectInitializer const& a_ppObjectInit);

	void BeginPlay() override;
	void OnPossess(APawn* a_pPawn) override;

	class UBlackboardComponent* GetBlackboardComp();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	class UBehaviorTreeComponent* m_pBehaviorTreeComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* m_pBehaviorTree;

private:
	class UBlackboardComponent* m_pBlackboardComp;

};
