#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	currentHealth = 100.0f;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// set health to the max value at the start
	ResetHealth();
}

void UHealthComponent::SetHealth(float health)
{
	float prevHealth = currentHealth;
	
	// keep the set value between 0 and max
	currentHealth = FMath::Clamp(health, 0.f, maxHealth);

	OnHealthUpdate.Broadcast(currentHealth, prevHealth, maxHealth);
}

void UHealthComponent::TakeDamage(float damage)
{
	if (bIsDead) return;

	float previousHealth = currentHealth;

	// set health to the decreased value
	SetHealth(currentHealth - damage);

	OnDamageReceived.Broadcast(currentHealth, previousHealth, maxHealth);

	if (currentHealth <= 0.f)
	{
		// die
		bIsDead = true;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::AddHealth(float health)
{
	// set health to the increased value
	SetHealth(currentHealth + health);
}

void UHealthComponent::ResetHealth()
{
	// set health to the max value
	currentHealth = maxHealth;
	SetHealth(currentHealth);
}

float UHealthComponent::GetHealth()
{
	return currentHealth;
}
