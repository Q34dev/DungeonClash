#include "PItem_HealthPotion.h"
#include "Kismet/GameplayStatics.h"
#include "DungeonClashCharacter.h"

void APItem_HealthPotion::PickedUp()
{
	// get the player character
	ACharacter* PCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (IsValid(PCharacter))
	{
		ADungeonClashCharacter* DCC = Cast<ADungeonClashCharacter>(PCharacter);
		if (IsValid(DCC))
		{
			// inform the player that the potion got picked up
			DCC->PickedUpAHealthPotion();
		}
	}
}
