#include "EnemyCharacter.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HealthBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyManager.h"
#include "Kismet/GameplayStatics.h"

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

	// Create the attack collision component
	AttackCol = CreateDefaultSubobject<USphereComponent>("AttackCollision");
	AttackCol->SetupAttachment(GetMesh(), "Attack_Socket");
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

	// bind the attack collision overlap method
	AttackCol->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnAttackOverlapBegin);
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
	
	// stop moving
	GetCharacterMovement()->StopMovementImmediately();

	bShouldDealDamage = false;

	// disable sword collision
	if (IsValid(AttackCol)) AttackCol->SetGenerateOverlapEvents(bShouldDealDamage);

	// play the attack animation
	if (IsValid(am_Attack)) GetMesh()->GetAnimInstance()->Montage_Play(am_Attack);
}

void AEnemyCharacter::OnSlashBegin()
{
	bShouldDealDamage = true;

	// enable sword collision
	if (IsValid(AttackCol)) AttackCol->SetGenerateOverlapEvents(bShouldDealDamage);
}

void AEnemyCharacter::OnSlashEnd()
{
	bShouldDealDamage = false;

	// disable sword collision
	if (IsValid(AttackCol)) AttackCol->SetGenerateOverlapEvents(bShouldDealDamage);
}

void AEnemyCharacter::Hit(UHealthComponent* hitActor)
{
	// deal damage to the hit actor
	hitActor->TakeDamage(attackDamage);
}

void AEnemyCharacter::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bShouldDealDamage) return;
	if (!IsValid(OtherActor)) return;
	if (OtherActor == this) return;
	
	// check if the hit actor has a health component
	UHealthComponent* hitHealthComp = OtherActor->GetComponentByClass<UHealthComponent>();
	if (!IsValid(hitHealthComp)) return;

	// check if the hit collision component was the hitbox of the hit actor
	if (hitHealthComp->Hitbox != OtherComp) return;

	Hit(hitHealthComp);
}

void AEnemyCharacter::OnHealthUpdate(float newHealth, float previousHealth, float maxHealth)
{
	if (newHealth < previousHealth)
	{ // enemy was damaged

		// show the health bar
		HealthBarWidgetComp->SetVisibility(true);

		// update the health bar
		if (IsValid(HealthBar)) HealthBar->OnDamageReceived(newHealth, previousHealth, maxHealth);
	}
}

void AEnemyCharacter::OnDeath()
{
	// get the enemy manager
	AActor* EnemyManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass());
	if (IsValid(EnemyManagerActor))
	{
		AEnemyManager* EnemyManager = Cast<AEnemyManager>(EnemyManagerActor);
		if (IsValid(EnemyManager))
		{
			// inform the enemy manager of the death
			EnemyManager->OnEnemyDied(this);
		}
	}

	// destroy the enemy object
	Destroy();
}
