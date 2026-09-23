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

	// An array of gateway barriers that block the room entrances and exits
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RoomManagement")
	TArray<AActor*> GatewayBarriers;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// A method to activate/deactivate the gateway barriers
	void SetBarriersActive(bool active);

	// Entrance trigger overlap method
	UFUNCTION()
	void OnEntranceTriggerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
