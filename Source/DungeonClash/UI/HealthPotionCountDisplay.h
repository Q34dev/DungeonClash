#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthPotionCountDisplay.generated.h"

class UTextBlock;

UCLASS()
class DUNGEONCLASH_API UHealthPotionCountDisplay : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthPotionCountText = nullptr;

private:
	void BindPlayerEvents();
	void UnbindPlayerEvents();
	
	UFUNCTION()
	void UpdateDisplayedHealthPotionCount();
};
