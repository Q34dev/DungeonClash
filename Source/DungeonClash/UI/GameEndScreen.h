#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndScreen.generated.h"

UCLASS()
class DUNGEONCLASH_API UGameEndScreen : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION()
	void OnGameEnded();
};
