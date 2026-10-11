#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "DungeonClashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	if (IsValid(PC = GetPlayerController()))
	{
		// enable the mouse cursor
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
}

void AGameManager::PlayerDied()
{
	// the game has ended (player lost)
	GameEnded(false);
}

void AGameManager::PlayerTriggeredWinStateBox()
{
	// the game has ended (player won)
	GameEnded(true);
}

void AGameManager::RestartButtonPressed()
{
	if (IsValid(GetWorld()))
	{
		// restart the game (by opening the level again)
		UGameplayStatics::OpenLevel(GetWorld(), GameLevelName);
	}
}

APlayerController* AGameManager::GetPlayerController()
{
	if (IsValid(PC)) return PC;
	// if player controller not assigned yet:

	PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(PC)) return nullptr;
	
	return PC;
}

ADungeonClashCharacter* AGameManager::GetPlayerCharacter()
{
	if (IsValid(DCC)) return DCC;
	// if player character not assigned yet:

	ACharacter* PCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!IsValid(PCharacter)) return nullptr;

	DCC = Cast<ADungeonClashCharacter>(PCharacter);
	if (!IsValid(DCC)) return nullptr;
	
	return DCC;
}
