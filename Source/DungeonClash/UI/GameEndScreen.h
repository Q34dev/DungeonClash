#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndScreen.generated.h"

class AGameManager;
class UCanvasPanel;
class UButton;

UCLASS()
class DUNGEONCLASH_API UGameEndScreen : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION()
	void OnGameEnded(bool playerWon);

	UFUNCTION()
	void OnRestartButtonPressed();

	UPROPERTY()
	AGameManager* GameManager;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr <UCanvasPanel> PlayerWonPanel = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr <UCanvasPanel> PlayerLostPanel = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr <UButton> ButtonRestartWon = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr <UButton> ButtonRestartLost = nullptr;
};
