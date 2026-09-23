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

			if (spawnedEnemiesCount == targetSpawnCount)
			{
				// stop when reached the required enemy spawn count
				break;
			}

			// get the enemy from the pool
			AEnemyCharacter* PooledEnemy = Cast<AEnemyCharacter>(EnemyPool[i]);

			// disable collision temporarily (in case of colliding with another actor)
			PooledEnemy->SetActorEnableCollision(false);

			// put the enemy at a random spawn point's position
			AActor* RandSpawnPoint = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
			PooledEnemy->TeleportTo(RandSpawnPoint->GetActorLocation(), RandSpawnPoint->GetActorRotation());

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

	// begin the room's first wave
	StartWave(0);
}

void AEnemyManager::OnEnemyDied(class AEnemyCharacter* DeadEnemy)
{
	// update the enemy count
	enemiesLeftInWave--;

	if (enemiesLeftInWave <= 0)
	{ // when all enemies in the wave died

		if (IsValid(CurrentEnemyRoom))
		{
			CurrentEnemyRoom->OnAllWavesFinished();
		}
	}
}
