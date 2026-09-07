#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthUpdate, float, newHealth, float, previousHealth, float, maxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCLASH_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY()
	UPrimitiveComponent* Hitbox;

	UFUNCTION(BlueprintCallable)
	virtual void SetHealth(float health);

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float damage);

	UFUNCTION(BlueprintCallable)
	virtual void AddHealth(float health);

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float maxHealth;

	UPROPERTY(BlueprintReadOnly)
	float currentHealth;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthUpdate OnHealthUpdate;
};
