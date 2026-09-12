#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerMeleeCombatComponent.generated.h"

class ADungeonClashCharacter;
class UHealthComponent;
class USoundBase;

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
	UAnimMontage* am_AttackCombo;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* am_AttackFinish;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float attackMontageBlendOutTime = .2f;
	UPROPERTY(EditAnywhere, Category = "Animation")
	float attackFinishMontageStartTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float attackComboDamage = 1.f;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float attackFinishDamage = 1.f;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* sb_AttackCombo;
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* sb_AttackFinish;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking;
	UPROPERTY(BlueprintReadWrite)
	bool bIsBufferingAttack;
	UPROPERTY(BlueprintReadWrite)
	bool bShouldDealDamage;

	UPROPERTY(BlueprintReadWrite)
	int attackComboIndex;

	void Attack();

	void StartAttack();
	void EndAttack();
	
	void OnSlashBegin(); // Anim Notify State Start
	void OnSlashEnd();  // Anim Notify State End
	void OnAttackEnd(); // Anim Notify

	void Hit(UHealthComponent* hitActor);

	// Sword collision
	UFUNCTION()
	void OnSwordOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void PlaySound(USoundBase* SB);
};
