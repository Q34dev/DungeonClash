#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMeleeCombatComponent.generated.h"

class ADungeonClashCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCLASH_API UPlayerMeleeCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMeleeCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	ADungeonClashCharacter* parentCharacter;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* am_Attack;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking;
	UPROPERTY(BlueprintReadWrite)
	bool bIsBufferingAttack;

	void Attack();

	void StartAttack();
	void EndAttack();
	
	void OnSlashBegin(); // Anim Notify State Start
	void OnSlashEnd();  // Anim Notify State End
	void OnAttackEnd(); // Anim Notify
};
