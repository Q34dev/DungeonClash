#pragma once

#include "CoreMinimal.h"
#include "PickupableItem.h"
#include "PItem_HealthPotion.generated.h"

UCLASS()
class DUNGEONCLASH_API APItem_HealthPotion : public APickupableItem
{
	GENERATED_BODY()
	
	// Overrided item pick up method
	void PickedUp() override;
};
