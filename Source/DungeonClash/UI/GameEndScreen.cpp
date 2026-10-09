#include "UI/GameEndScreen.h"
#include "Kismet/GameplayStatics.h"
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

void UGameEndScreen::OnGameEnded()
{
	// show the game end screen
	SetVisibility(ESlateVisibility::Visible);
}
