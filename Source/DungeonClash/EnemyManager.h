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

	// The number of enemies to spawn in each wave
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Management")
	TArray<int> enemySpawnCounts;

	// The number of waves remaining in the current enemy room
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Management")
	int wavesLeft = 1;

	// The index of the currently active wave
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Management")
	int currentWaveIndex = 0;

	// The number of enemies remaining in the current wave
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Management")
	int enemiesLeftInWave = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The enemy room that the player is currently in
	UPROPERTY()
	class AEnemyRoomManager* CurrentEnemyRoom;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Methods for managing enemy waves
	void StartWave(int waveIndex);
	void StartNextWave();

	// A method to set the room that the player is in
	void SetCurrentEnemyRoom(AEnemyRoomManager* Room);

	// A method to react to an enemy's death
	void OnEnemyDied(class AEnemyCharacter* DeadEnemy);
};
