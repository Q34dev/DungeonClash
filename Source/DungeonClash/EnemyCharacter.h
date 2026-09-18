#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class DUNGEONCLASH_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Health component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UHealthComponent* HealthComp;

	// Health bar widget component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* HealthBarWidgetComp;
	
	UPROPERTY()
	class UHealthBar* HealthBar;

	// Attack collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* AttackCol;

	// Attack animation montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* am_Attack;

	void Attack();

	// Attack collision overlap method
	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	UFUNCTION()
	void OnHealthUpdate(float newHealth, float previousHealth, float maxHealth);
	
	UFUNCTION()
	void OnDeath();
};
