#include "UI/PlayerHealthBar.h"
#include "DungeonClashCharacter.h"

void UPlayerHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	BindPlayerEvents();
}

void UPlayerHealthBar::NativeDestruct()
{
	UnbindPlayerEvents();

	Super::NativeDestruct();
}

void UPlayerHealthBar::BindPlayerEvents()
{
	if (const APlayerController* LocalPlayer = GetOwningPlayer())
	{
		if (const APawn* LocalPawn = LocalPlayer->GetPawn())
		{
			if (const ADungeonClashCharacter* CustomLocalPawn = Cast<ADungeonClashCharacter>(LocalPawn))
			{
				CustomLocalPawn->GetOnHealthChangedEvent().AddUObject(this, &UHealthBar::OnHealthChanged);
			}
		}
	}
}

void UPlayerHealthBar::UnbindPlayerEvents()
{
	if (const APlayerController* LocalPlayer = GetOwningPlayer())
	{
		if (const APawn* LocalPawn = LocalPlayer->GetPawn())
		{
			if (const ADungeonClashCharacter* CustomLocalPawn = Cast<ADungeonClashCharacter>(LocalPawn))
			{
				CustomLocalPawn->GetOnHealthChangedEvent().RemoveAll(this);
			}
		}
	}
}