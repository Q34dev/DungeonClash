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
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyManager::StartWave(int waveIndex)
{
	// check if the given wave index is inside the bounds
	if (waveIndex < 0 || waveIndex >= enemySpawnCounts.Num()) return;

	// update the current wave index
	currentWaveIndex = waveIndex;

	int targetSpawnCount = enemySpawnCounts[waveIndex];

	// set the enemy count
	enemiesLeftInWave = targetSpawnCount;

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

			// select a spawn point index
			int spawnPointIndex = 0;
			if (i < CurrentEnemyRoom->GetSpawnPoints().Num()) spawnPointIndex = i;

			// put the enemy at the spawn point's position
			AActor* SpawnPoint = CurrentEnemyRoom->GetSpawnPoints()[spawnPointIndex];
			PooledEnemy->TeleportTo(SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());

			// reenable collision
			PooledEnemy->SetActorEnableCollision(true);

			// increase the spawned enemies count
			spawnedEnemiesCount++;
		}
	}
}

void AEnemyManager::StartNextWave()
{
	StartWave(currentWaveIndex + 1);
}

void AEnemyManager::SetCurrentEnemyRoom(AEnemyRoomManager* Room)
{
	CurrentEnemyRoom = Room;

	if (!IsValid(CurrentEnemyRoom)) return;
	// when entered the room:

	// set the wave count
	wavesLeft = enemySpawnCounts.Num();

	// begin the room's first wave
	StartWave(0);
}

void AEnemyManager::OnEnemyDied(class AEnemyCharacter* DeadEnemy)
{
	// update the enemy count
	enemiesLeftInWave--;

	if (enemiesLeftInWave <= 0)
	{ // when all enemies in the wave died
		// the wave is finished

		// update the wave count
		wavesLeft--;

		if (wavesLeft <= 0)
		{ // when all the waves finished

			if (IsValid(CurrentEnemyRoom))
			{
				CurrentEnemyRoom->OnAllWavesFinished();
			}
		}
		else
		{ // if not all waves finished

			StartNextWave();
		}
	}
}
