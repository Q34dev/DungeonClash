#include "HealthBar.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	SetHealthMaterialParameter(HealthParameterName, 1.0f);
	SetHealthMaterialParameter(PreviousHealthParameterName, 1.0f);
}

void UHealthBar::OnDamageReceived(const float newHealth, const float previousHealth, const float maxHealth)
{
	normalizedHealthVal = newHealth / maxHealth;
	normalizedPreviousHealthVal = previousHealth / maxHealth;

	SetHealthMaterialParameter(HealthParameterName, normalizedHealthVal);

	if (!IsValid(DamageAnim)) return;

	PlayAnimation(DamageAnim);
}

void UHealthBar::ResetHealthBar()
{
	normalizedHealthVal = 1.f;
	normalizedPreviousHealthVal = 1.f;

	SetHealthMaterialParameter(HealthParameterName, 1.0f);
	SetHealthMaterialParameter(PreviousHealthParameterName, 1.0f);
}

void UHealthBar::LerpCachedHealthValue(const float Value)
{
	const UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	normalizedPreviousHealthVal = FMath::FInterpTo(normalizedPreviousHealthVal, normalizedHealthVal, World->GetDeltaSeconds(), interpSpeed);

	SetHealthMaterialParameter(PreviousHealthParameterName, normalizedPreviousHealthVal);
}

void UHealthBar::SetHealthMaterialParameter(const FName& ParameterName, const float Value)
{
	if (!IsValid(HealthBarImage)) return;

	UMaterialInstanceDynamic* BarMaterial = HealthBarImage->GetDynamicMaterial();
	if (!IsValid(BarMaterial)) return;

	BarMaterial->SetScalarParameterValue(ParameterName, Value);
}
