#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupableItem.generated.h"

UCLASS()
class DUNGEONCLASH_API APickupableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupableItem();

	// The pickup trigger collider
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* PickupTrigger;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Executed when player picks up the item
	virtual void PickedUp();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Entrance trigger overlap method
	UFUNCTION()
	void OnPickupTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
