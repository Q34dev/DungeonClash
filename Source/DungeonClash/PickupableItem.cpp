#include "PickupableItem.h"
#include "Components/BoxComponent.h"
#include "DungeonClashCharacter.h"

// Sets default values
APickupableItem::APickupableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the pickup trigger collision component
	PickupTrigger = CreateDefaultSubobject<UBoxComponent>("PickupTrigger");

	// Bind the pickup trigger overlap method
	PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupableItem::OnPickupTriggerOverlapBegin);
}

// Called when the game starts or when spawned
void APickupableItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickupableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupableItem::PickedUp()
{
}

void APickupableItem::OnPickupTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor)) return;
	if (!Cast<ADungeonClashCharacter>(OtherActor)) return;
	// if player overlapped with the trigger

	PickedUp();

	// destroy the item
	Destroy();
}

