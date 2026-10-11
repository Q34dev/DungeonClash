#include "WinStateTriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"
#include "DungeonClashCharacter.h"

AWinStateTriggerBox::AWinStateTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &AWinStateTriggerBox::OnBoxOverlapBegin);
}

void AWinStateTriggerBox::OnBoxOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!IsValid(OtherActor)) return;
    if (OtherActor == this) return;

	if (!Cast<ADungeonClashCharacter>(OtherActor)) return;
	// if the player overlapped with this trigger box

	// get the game manager actor
	AActor* GameManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass());
	if (!IsValid(GameManagerActor)) return;
	
	// get the game manager
	AGameManager* GameManager = Cast<AGameManager>(GameManagerActor);
	if (!IsValid(GameManager)) return;
	
	// inform the game manager that the player has triggered the win state trigger box
	GameManager->PlayerTriggeredWinStateBox();
}
