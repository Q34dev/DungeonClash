#include "EnemyCharacter.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HealthBar.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the health component
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("EnemyHealthComponent"));

	// Create the health bar widget component
	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthBar"));
	HealthBarWidgetComp->SetupAttachment(GetMesh());
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

	if (IsValid(HealthBarWidgetComp))
	{
		// get the health bar class
		if (IsValid(HealthBarWidgetComp->GetWidget()))
			HealthBar = Cast<UHealthBar>(HealthBarWidgetComp->GetWidget());

		// hide the health bar at the start
		HealthBarWidgetComp->SetVisibility(false);
	}
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

void AEnemyCharacter::Attack()
{
	if (!IsValid(GetMesh()) || !IsValid(GetMesh()->GetAnimInstance())) return;
	
	// play the attack animation
	if (IsValid(am_Attack)) GetMesh()->GetAnimInstance()->Montage_Play(am_Attack);
}

void AEnemyCharacter::OnHealthUpdate(float newHealth, float previousHealth, float maxHealth)
{
	if (newHealth < previousHealth)
	{ // enemy was damaged

		// show the health bar
		HealthBarWidgetComp->SetVisibility(true);

		// update the health bar
		if (IsValid(HealthBar)) HealthBar->OnDamageReceived(newHealth, previousHealth, maxHealth);

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s hit! Health: %d -> %d"), *GetActorNameOrLabel(), FMath::CeilToInt(previousHealth), FMath::CeilToInt(newHealth)));
	}
}

void AEnemyCharacter::OnDeath()
{
	// destroy the enemy object
	Destroy();
}
