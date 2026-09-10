#include "MainHUD.h"
#include "Blueprint/UserWidget.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(HudTemplate)) return;
	
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!IsValid(PlayerController)) return;
	
	HUDWidget = CreateWidget<UUserWidget>(PlayerController, HudTemplate);
	HUDWidget->AddToViewport();
}
