#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnded, bool, playerWon);

UCLASS()
class DUNGEONCLASH_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the player won or lost the game
	UFUNCTION()
	void GameEnded(bool playerWon);

	// Called when the player died
	void PlayerDied();

	UPROPERTY(BlueprintAssignable)
	FOnGameEnded OnGameEnded;
};
