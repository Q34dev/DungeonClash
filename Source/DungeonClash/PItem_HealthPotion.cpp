#include "PItem_HealthPotion.h"

void APItem_HealthPotion::PickedUp()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Health potion picked up");
}
