#include "EnemyRoomManager.h"
#include "Components/BoxComponent.h"
#include "DungeonClashCharacter.h"
#include "EnemyManager.h"

// Sets default values
AEnemyRoomManager::AEnemyRoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the entrance trigger collision component
	EntranceTrigger = CreateDefaultSubobject<UBoxComponent>("EntranceTrigger");

	// Bind the entrance trigger overlap method
	EntranceTrigger->OnComponentEndOverlap.AddDynamic(this, &AEnemyRoomManager::OnEntranceTriggerOverlapEnd);
}

// Called when the game starts or when spawned
void AEnemyRoomManager::BeginPlay()
{
	Super::BeginPlay();

	// deactivate the barriers at the start to let the player in
	SetBarriersActive(false);
}

// Called every frame
void AEnemyRoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyRoomManager::SetBarriersActive(bool active)
{
	for (AActor* Barrier : GatewayBarriers)
	{ // iterate through all gateway barriers

		// show/hide the barrier
		Barrier->SetActorHiddenInGame(!active);
		
		// enable/disable barrier collision
		Barrier->SetActorEnableCollision(active);
	}
}

void AEnemyRoomManager::OnEntranceTriggerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor)) return;
	if (!Cast<ADungeonClashCharacter>(OtherActor)) return;
	// if player finished overlapping with the trigger

	// activate the barriers to lock the player inside the room
	SetBarriersActive(true);

	if (IsValid(EnemyManager))
	{
		// set this room as the current enemy room
		EnemyManager->SetCurrentEnemyRoom(this);
	}
}
