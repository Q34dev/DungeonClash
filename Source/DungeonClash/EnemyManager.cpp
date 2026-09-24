#include "EnemyManager.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyRoomManager.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	EnemyPool.Empty();

	waveCount = 0;
	wavesLeft = 0;
	currentWaveIndex = 0;
	currentEnemyIndex = 0;
	enemiesLeftInWave = 0;
	CurrentEnemyRoom = nullptr;
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyManager::SpawnEnemy()
{
	// get the enemy spawn point
	AActor* SpawnPoint = CurrentEnemyRoom->GetEnemySpawnPoint(currentWaveIndex, currentEnemyIndex);
	if (!IsValid(SpawnPoint)) return;

	FVector SpawnLocation = SpawnPoint->GetActorLocation();
	FRotator SpawnRotation = SpawnPoint->GetActorRotation();
	
	AEnemyCharacter* SpawnedEnemy = nullptr;

	if (EnemyPool.Num() > 0)
	{ // if there are any inactive enemies available in the pool

		// get the last enemy from the pool
		// and remove that enemy from the pool
		SpawnedEnemy = EnemyPool.Pop();

		// teleport the enemy to the spawn location
		SpawnedEnemy->TeleportTo(SpawnLocation, SpawnRotation, false, true);

		// reactivate the enemy
		SpawnedEnemy->ActivateEnemy();
	}
	else
	{ // if there are no enemies left in the pool

		// spawn a new enemy actor at the spawn location:
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		GetWorld()->SpawnActor<AEnemyCharacter>(EnemyBlueprint, SpawnLocation, SpawnRotation, SpawnParameters);
	}
}

void AEnemyManager::StartWave(int waveIndex)
{
	// check if the given wave index is inside the bounds
	if (waveIndex < 0 || waveIndex >= waveCount) return;

	// check if the player is inside an enemy room
	if (!IsValid(CurrentEnemyRoom)) return;

	// update the current wave index
	currentWaveIndex = waveIndex;

	// reset the enemy index
	currentEnemyIndex = 0;

	// get the number of enemies to spawn in this wave
	int targetSpawnCount = CurrentEnemyRoom->GetEnemySpawnCountInWave(currentWaveIndex);

	// set the enemy count
	enemiesLeftInWave = targetSpawnCount;

	// check if there are any enemies to spawn
	if (targetSpawnCount <= 0) return;

	for (int i = 0; i < targetSpawnCount; i++)
	{
		// spawn the enemy
		SpawnEnemy();

		// set the next enemy index
		currentEnemyIndex++;
	}
}

void AEnemyManager::StartNextWave()
{
	StartWave(currentWaveIndex + 1);
}

void AEnemyManager::FinishWave()
{
	// update the wave count
	wavesLeft--;

	if (wavesLeft <= 0)
	{ // when all the waves finished

		if (IsValid(CurrentEnemyRoom))
		{
			CurrentEnemyRoom->OnAllWavesFinished();
		}

		waveCount = 0;
		wavesLeft = 0;
		currentWaveIndex = 0;
		currentEnemyIndex = 0;
		enemiesLeftInWave = 0;
		CurrentEnemyRoom = nullptr;
	}
	else
	{ // if not all waves finished

		StartNextWave();
	}
}

void AEnemyManager::SetCurrentEnemyRoom(AEnemyRoomManager* Room)
{
	CurrentEnemyRoom = Room;

	if (!IsValid(CurrentEnemyRoom)) return;
	// when entered the room:

	// set the wave count
	waveCount = CurrentEnemyRoom->GetWaveCount();
	wavesLeft = waveCount;

	// set the enemy index
	currentEnemyIndex = 0;
	enemiesLeftInWave = 0;

	// begin the room's first wave
	StartWave(0);
}

void AEnemyManager::OnEnemyDied(class AEnemyCharacter* DeadEnemy)
{
	// update the enemy count
	enemiesLeftInWave--;

	// add the enemy to the enemy pool
	EnemyPool.Add(DeadEnemy);

	if (enemiesLeftInWave <= 0)
	{ // when all enemies in the wave died
		
		FinishWave();
	}
}
