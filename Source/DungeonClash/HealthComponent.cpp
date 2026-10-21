#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

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
	// can't take damage if is already dead or is currently invincible
	if (bIsDead || bIsInvincible) return;

	float previousHealth = currentHealth;

	// set health to the decreased value
	SetHealth(currentHealth - damage);

	OnHealthChanged.Broadcast(currentHealth, previousHealth, maxHealth);

	// play the damage taken sound
	PlaySound(sb_DamageTaken, 1.f, .2f);

	if (currentHealth <= 0.f)
	{
		// die
		bIsDead = true;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::AddHealth(float health)
{
	float previousHealth = currentHealth;

	// set health to the increased value
	SetHealth(currentHealth + health);

	OnHealthChanged.Broadcast(currentHealth, previousHealth, maxHealth);
}

void UHealthComponent::ResetHealth()
{
	// set health to the max value
	currentHealth = maxHealth;
	SetHealth(currentHealth);

	bIsDead = false;
}

float UHealthComponent::GetHealth()
{
	return currentHealth;
}

bool UHealthComponent::HasFullHealth()
{
	return currentHealth >= maxHealth;
}

void UHealthComponent::SetIfInvincible(bool bSetInvincible)
{
	bIsInvincible = bSetInvincible;
}

void UHealthComponent::PlaySound(USoundBase* SB, float volumeMultiplier, float pitchRandomizeRange)
{
	if (!IsValid(SB)) return;
	if (!IsValid(GetWorld())) return;
	if (!IsValid(GetOwner())) return;

	float pitch = (pitchRandomizeRange > .0f) ? FMath::RandRange(1.f - pitchRandomizeRange, 1.f + pitchRandomizeRange) : 1.f;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SB, GetOwner()->GetActorLocation(), volumeMultiplier, pitch);
}
