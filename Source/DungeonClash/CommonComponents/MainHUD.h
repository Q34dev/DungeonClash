#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

UCLASS()
class DUNGEONCLASH_API AMainHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HudTemplate = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> HUDWidget = nullptr;
};
