#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class DUNGEONCLASH_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyManager();

	// A container that stores already spawned enemy actors
	TArray<AActor*> EnemyPool;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The enemy room that the player is currently in
	UPROPERTY()
	class AEnemyRoomManager* CurrentEnemyRoom;

	// The total number of all waves in the current enemy room
	int waveCount = 1;

	// The number of waves remaining in the current enemy room
	int wavesLeft = 1;

	// The index of the currently active wave
	int currentWaveIndex = 0;

	// The index of the enemy that's being spawned in the current wave
	int currentEnemyIndex = 0;

	// The number of enemies remaining in the current wave
	int enemiesLeftInWave = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Methods for managing enemy waves
	void StartWave(int waveIndex);
	void StartNextWave();
	void FinishWave();

	// A method to set the room that the player is in
	void SetCurrentEnemyRoom(AEnemyRoomManager* Room);

	// A method to react to an enemy's death
	void OnEnemyDied(class AEnemyCharacter* DeadEnemy);
};
