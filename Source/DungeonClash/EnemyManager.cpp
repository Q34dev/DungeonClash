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

	// add all enemies in the level to the pool
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemyPool);

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

	if (EnemyPool.Num() > 0)
	{ // if there are any enemies left in the pool

		int spawnedEnemiesCount = 0;
		for (int i = 0; i < EnemyPool.Num(); i++)
		{ // iterate through all enemies in the pool

			// check if the enemy still exists
			if (!IsValid(EnemyPool[i])) continue;

			if (spawnedEnemiesCount == targetSpawnCount)
			{
				// stop when reached the required enemy spawn count
				break;
			}

			// get the enemy from the pool
			AEnemyCharacter* PooledEnemy = Cast<AEnemyCharacter>(EnemyPool[i]);

			// disable collision temporarily (in case of colliding with another actor)
			PooledEnemy->SetActorEnableCollision(false);

			// get the enemy spawn point
			AActor* SpawnPoint = CurrentEnemyRoom->GetEnemySpawnPoint(currentWaveIndex, currentEnemyIndex);
			if (IsValid(SpawnPoint))
			{
				// teleport the enemy to the spawn point's location
				PooledEnemy->TeleportTo(SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
			}

			// reenable collision
			PooledEnemy->SetActorEnableCollision(true);

			// set the next enemy index
			currentEnemyIndex++;

			// increase the spawned enemies count
			spawnedEnemiesCount++;
		}
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

	if (enemiesLeftInWave <= 0)
	{ // when all enemies in the wave died
		
		FinishWave();
	}
}
