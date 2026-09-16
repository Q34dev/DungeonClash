#pragma once

#include "CoreMinimal.h"
#include "UI/HealthBar.h"
#include "PlayerHealthBar.generated.h"

UCLASS()
class DUNGEONCLASH_API UPlayerHealthBar : public UHealthBar
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BindPlayerEvents();
	void UnbindPlayerEvents();
};
