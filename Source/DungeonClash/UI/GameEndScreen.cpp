#include "UI/GameEndScreen.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"

void UGameEndScreen::NativeConstruct()
{
	Super::NativeConstruct();

	// get the game manager
	AActor* GameManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	if (IsValid(GameManagerActor))
	{
		GameManager = Cast<AGameManager>(GameManagerActor);
	}

	if (IsValid(GameManager))
	{
		// bind the game ended method
		GameManager->OnGameEnded.AddDynamic(this, &UGameEndScreen::OnGameEnded);
	}

	// bind the restart button method
	if (IsValid(ButtonRestartWon)) ButtonRestartWon->OnPressed.AddDynamic(this, &UGameEndScreen::OnRestartButtonPressed);
	if (IsValid(ButtonRestartLost)) ButtonRestartLost->OnPressed.AddDynamic(this, &UGameEndScreen::OnRestartButtonPressed);
}

void UGameEndScreen::NativeDestruct()
{
	if (!IsValid(GameManager))
	{
		// get the game manager
		AActor* GameManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
		if (IsValid(GameManagerActor))
		{
			GameManager = Cast<AGameManager>(GameManagerActor);
		}
	}

	if (IsValid(GameManager))
	{
		// unbind the game ended method
		GameManager->OnGameEnded.RemoveDynamic(this, &UGameEndScreen::OnGameEnded);
	}

	// unbind the restart button method
	if (IsValid(ButtonRestartWon)) ButtonRestartWon->OnPressed.RemoveDynamic(this, &UGameEndScreen::OnRestartButtonPressed);
	if (IsValid(ButtonRestartLost)) ButtonRestartLost->OnPressed.RemoveDynamic(this, &UGameEndScreen::OnRestartButtonPressed);

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

void UGameEndScreen::OnRestartButtonPressed()
{
	if (IsValid(GameManager))
	{
		// inform the game manager that the restart game button has been pressed
		GameManager->RestartButtonPressed();
	}
}
