#include "HealthBar.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "DungeonClashCharacter.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	BindPlayerEvents();

	SetHealthMaterialParameter(HealthParameterName, 1.0f);
	SetHealthMaterialParameter(PreviousHealthParameterName, 1.0f);
}

void UHealthBar::NativeDestruct()
{
	UnbindPlayerEvents();

	Super::NativeDestruct();
}

void UHealthBar::BindPlayerEvents()
{
	if (const APlayerController* LocalPlayer = GetOwningPlayer())
	{
		if (const APawn* LocalPawn = LocalPlayer->GetPawn())
		{
			if (const ADungeonClashCharacter* CustomLocalPawn = Cast<ADungeonClashCharacter>(LocalPawn))
			{
				CustomLocalPawn->GetOnHealthInitEvent().AddUObject(this, &UHealthBar::OnHealthInit);
				CustomLocalPawn->GetOnDamageReceivedEvent().AddUObject(this, &UHealthBar::OnDamageReceived);
			}
		}
	}
}

void UHealthBar::UnbindPlayerEvents()
{
	if (const APlayerController* LocalPlayer = GetOwningPlayer())
	{
		if (const APawn* LocalPawn = LocalPlayer->GetPawn())
		{
			if (const ADungeonClashCharacter* CustomLocalPawn = Cast<ADungeonClashCharacter>(LocalPawn))
			{
				CustomLocalPawn->GetOnHealthInitEvent().RemoveAll(this);
				CustomLocalPawn->GetOnDamageReceivedEvent().RemoveAll(this);
			}
		}
	}
}

void UHealthBar::OnHealthInit(const float maxHealth)
{
	maxHealthVal = maxHealth;
	normalizedHealthVal = 1.f;
}

void UHealthBar::OnDamageReceived(const float newHealth, const float previousHealth)
{
	normalizedHealthVal = newHealth / maxHealthVal;
	normalizedPreviousHealthVal = previousHealth / maxHealthVal;

	SetHealthMaterialParameter(HealthParameterName, normalizedHealthVal);

	if (!IsValid(DamageAnim)) return;

	PlayAnimation(DamageAnim);
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
