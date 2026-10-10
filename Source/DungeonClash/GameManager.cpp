#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::GameEnded(bool playerWon)
{
	OnGameEnded.Broadcast(playerWon);
}

void AGameManager::PlayerDied()
{
	// the game has ended (player lost)
	GameEnded(false);
}
