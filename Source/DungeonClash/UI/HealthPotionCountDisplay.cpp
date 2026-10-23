#include "UI/HealthPotionCountDisplay.h"
#include "DungeonClashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UHealthPotionCountDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	BindPlayerEvents();
}

void UHealthPotionCountDisplay::NativeDestruct()
{
	UnbindPlayerEvents();

	Super::NativeDestruct();
}

void UHealthPotionCountDisplay::BindPlayerEvents()
{
	if (APlayerController* LocalPlayer = GetOwningPlayer())
	{
		if (APawn* LocalPawn = LocalPlayer->GetPawn())
		{
			if (ADungeonClashCharacter* CustomLocalPawn = Cast<ADungeonClashCharacter>(LocalPawn))
			{
				CustomLocalPawn->OnHealthPotionCountChanged.AddDynamic(this, &UHealthPotionCountDisplay::UpdateDisplayedHealthPotionCount);
			}
		}
	}
}

void UHealthPotionCountDisplay::UnbindPlayerEvents()
{
	if (APlayerController* LocalPlayer = GetOwningPlayer())
	{
		if (APawn* LocalPawn = LocalPlayer->GetPawn())
		{
			if (ADungeonClashCharacter* CustomLocalPawn = Cast<ADungeonClashCharacter>(LocalPawn))
			{
				CustomLocalPawn->OnHealthPotionCountChanged.RemoveDynamic(this, &UHealthPotionCountDisplay::UpdateDisplayedHealthPotionCount);
			}
		}
	}
}

void UHealthPotionCountDisplay::UpdateDisplayedHealthPotionCount()
{
	// get the player character
	ACharacter* PCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!IsValid(PCharacter)) return;
	ADungeonClashCharacter* DCC = Cast<ADungeonClashCharacter>(PCharacter);
	if (!IsValid(DCC)) return;

	// get the current health potion count
	int healthPotionCount = DCC->GetHealthPotionCount();

	// set the text value
	HealthPotionCountText->SetText(FText::FromString(FString::FromInt(healthPotionCount)));
}
