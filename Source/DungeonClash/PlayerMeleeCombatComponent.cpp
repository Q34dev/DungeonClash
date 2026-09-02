#include "PlayerMeleeCombatComponent.h"
#include "DungeonClashCharacter.h"

// Sets default values for this component's properties
UPlayerMeleeCombatComponent::UPlayerMeleeCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPlayerMeleeCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPlayerMeleeCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerMeleeCombatComponent::Attack()
{
	ADungeonClashCharacter* parentCharacter = Cast<ADungeonClashCharacter>(GetOwner());
	if (!parentCharacter) return;

	// play the attack animation
	parentCharacter->GetMesh()->GetAnimInstance()->Montage_Play(am_Attack);
}

void UPlayerMeleeCombatComponent::OnSlashBegin()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "SLASH BEGIN");
}

void UPlayerMeleeCombatComponent::OnSlashEnd()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "SLASH END");
}

void UPlayerMeleeCombatComponent::OnAttackEnd()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, "ATTACK END");
}
