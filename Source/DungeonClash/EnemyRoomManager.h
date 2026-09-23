#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRoomManager.generated.h"

USTRUCT(Atomic)
struct FEnemySpawnData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int enemyTypeIndex = 0;

	UPROPERTY(EditAnywhere)
	int spawnPointIndex = 0;
};

USTRUCT(Atomic)
struct FWaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FEnemySpawnData> EnemySpawnDataArray;
};

UCLASS()
class DUNGEONCLASH_API AEnemyRoomManager : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AEnemyRoomManager();

	// A trigger collider for detecting player entrance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* EntranceTrigger;

	// The enemy manager
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RoomManagement")
	class AEnemyManager* EnemyManager;

	// The parent object of attached enemy spawn points
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RoomManagement")
	AActor* SpawnPointsParent;

	// The enemy spawn points
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Management")
	TArray<AActor*> SpawnPoints;

	// An array of gateway barriers that block the room entrances and exits
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RoomManagement")
	TArray<AActor*> GatewayBarriers;

	// An array of arrays of enemy spawn data that contain info about where and which enemies to spawn
	UPROPERTY(EditAnywhere, Category = "RoomManagement")
	TArray<FWaveData> WaveDataArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Are enemy waves currently spawning in this room
	bool bIsEnemyRoomActive;

	// Did the player defeat all the enemy waves in this room
	bool bIsEnemyRoomFinished;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// A method to activate/deactivate the gateway barriers
	void SetBarriersActive(bool active);

	// Entrance trigger overlap method
	UFUNCTION()
	void OnEntranceTriggerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// A method to signal when the player finished all the room's enemy waves
	void OnAllWavesFinished();

	// Returns the number of enemy waves in this room
	int GetWaveCount();

	// Returns the number of enemies in a specific wave
	int GetEnemySpawnCountInWave(int waveIndex);

	// Returns the spawn point for a specific enemy in a specific wave
	AActor* GetEnemySpawnPoint(int waveIndex, int enemyIndex);
};
