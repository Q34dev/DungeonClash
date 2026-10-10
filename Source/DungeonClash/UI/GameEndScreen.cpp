#include "UI/GameEndScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "GameManager.h"

void UGameEndScreen::NativeConstruct()
{
	Super::NativeConstruct();

	AActor* GameManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	if (!IsValid(GameManagerActor)) return;

	AGameManager* GameManager = Cast<AGameManager>(GameManagerActor);
	if (!IsValid(GameManager)) return;

	GameManager->OnGameEnded.AddDynamic(this, &UGameEndScreen::OnGameEnded);
}

void UGameEndScreen::NativeDestruct()
{
	AActor* GameManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	if (!IsValid(GameManagerActor)) return;

	AGameManager* GameManager = Cast<AGameManager>(GameManagerActor);
	if (!IsValid(GameManager)) return;

	GameManager->OnGameEnded.RemoveDynamic(this, &UGameEndScreen::OnGameEnded);

	Super::NativeDestruct();
}

void UGameEndScreen::OnGameEnded(bool playerWon)
{
	if (playerWon)
	{ // if the player won the game

		// show the player won panel
		if (IsValid(PlayerWonPanel))
			PlayerWonPanel->SetVisibility(ESlateVisibility::Visible);

		// hide the player lost panel
		if (IsValid(PlayerLostPanel))
			PlayerLostPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{ // if the player lost the game

		// show the player lost panel
		if (IsValid(PlayerLostPanel))
			PlayerLostPanel->SetVisibility(ESlateVisibility::Visible);

		// hide the player won panel
		if (IsValid(PlayerWonPanel))
			PlayerWonPanel->SetVisibility(ESlateVisibility::Hidden);
	}

	// show the game end screen
	SetVisibility(ESlateVisibility::Visible);
}
