#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UImage;

UCLASS(Abstract)
class DUNGEONCLASH_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DamageAnim = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> HealthBarImage = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Material Parameters")
	FName HealthParameterName = FName("CurrentHealth");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Material Parameters")
	FName PreviousHealthParameterName = FName("PreviousHealth");

private:

	void BindPlayerEvents();
	void UnbindPlayerEvents();

	void OnHealthInit(const float maxHealth);
	void OnDamageReceived(const float newHealth, const float previousHealth);
	
	UFUNCTION(BlueprintCallable)
	void LerpCachedHealthValue(const float Value);
	
	void SetHealthMaterialParameter(const FName& ParameterName, const float Value);

	UPROPERTY(EditAnywhere, Category = "Interpolation")
	float interpSpeed = 10.f;

	float maxHealthVal = 1.f;

	// normalized health values - between 0 & 1
	float normalizedHealthVal = 1.f;
	float normalizedPreviousHealthVal = 1.f;
};
