#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnded, bool, playerWon);

class ADungeonClashCharacter;

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

	// Called when the player triggered the win state trigger box
	void PlayerTriggeredWinStateBox();

	// Called when a restart game button on a UI screen has been pressed
	void RestartButtonPressed();

	// The Player Controller
	UPROPERTY()
	APlayerController* PC = nullptr;
	UFUNCTION()
	APlayerController* GetPlayerController();

	// The Player (Dungeon Clash) Character
	UPROPERTY()
	ADungeonClashCharacter* DCC = nullptr;
	UFUNCTION()
	ADungeonClashCharacter* GetPlayerCharacter();

	UPROPERTY(BlueprintAssignable)
	FOnGameEnded OnGameEnded;

	UPROPERTY(EditAnywhere, Category = "Game Management")
	FName GameLevelName;

	// has the game already ended
	bool bGameEnded = false;
};
