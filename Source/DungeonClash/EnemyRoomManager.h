#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRoomManager.generated.h"

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

	// Returns the enemy spawn points array
	TArray<AActor*> GetSpawnPoints();
};
