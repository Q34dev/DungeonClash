#include "EnemyCharacter.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the health component
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("EnemyHealthComponent"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// assign the hitbox
	HealthComp->Hitbox = GetCapsuleComponent();

	// assign the Health Component functions
	HealthComp->OnHealthUpdate.AddDynamic(this, &AEnemyCharacter::OnHealthUpdate);
	HealthComp->OnDeath.AddDynamic(this, &AEnemyCharacter::OnDeath);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::OnHealthUpdate(float newHealth, float previousHealth, float maxHealth)
{
	if (newHealth < previousHealth)
	{ // enemy was damaged

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s hit! Health: %d -> %d"), *GetActorNameOrLabel(), FMath::CeilToInt(previousHealth), FMath::CeilToInt(newHealth)));
	}
}

void AEnemyCharacter::OnDeath()
{
	// destroy the enemy object
	Destroy();
}
