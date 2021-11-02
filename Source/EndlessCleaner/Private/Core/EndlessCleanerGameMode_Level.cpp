// Copyright Epic Games, Inc. All Rights Reserved.


#include "Core/EndlessCleanerGameMode_Level.h"
#include "Player/EndlessCleanerPlayerController.h"
#include "Player/EndlessCleanerCharacter.h"
#include "Gameplay/Platforms/PlatformModule.h"
#include "Gameplay/Platforms/PlatformsContainer.h"
#include "Gameplay/EnemyFactory.h"
#include "Gameplay/Enemy.h"
#include "UI/InGameUIWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Audio/AudioManager.h"


// Sets default values
AEndlessCleanerGameMode_Level::AEndlessCleanerGameMode_Level()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Variables values
	NumberOfInitialPlatforms = 5;
	VisiblePlatformNumber = 9;
	RespawnTimer = 3.0f;
	SpawnStartTimer = 2.0f;
	InitialPlayerLives = 3;
	PickupsRushPlatformsCount = 2;
	ObstaclesRushPlatformsCount = 2;

	CoinsMultiplier = 100;
	SpeedMultiplier = 1000;
}

// Called when the game starts or when spawned
void AEndlessCleanerGameMode_Level::StartPlay()
{
	Super::StartPlay();

	// Set Initial Game State
	GameState = EGameState::VE_PreparePlatforms;

	PlayerController = Cast<AEndlessCleanerPlayerController>(GetWorld()->GetFirstPlayerController());

	Player = Cast<AEndlessCleanerCharacter>(PlayerController->GetCharacter());

	InitializeGame();

	PlayerController->SetInitialLives(InitialPlayerLives);

	GameState = EGameState::VE_PrepareGame;

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AEndlessCleanerGameMode_Level::OnRespawn, 0.1f, false);

#pragma region Debug
	SetDebugLogState(bAllowDebugLogs);
	SetDebugScreenLogState(bPrintLogsOnScreen);
#pragma endregion
}

// Called every frame
void AEndlessCleanerGameMode_Level::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(Player) || GameState != EGameState::VE_Running) return;

	if (IsValid(PlatformToCheck))
	{
		if (Player->GetActorLocation().X > PlatformToCheck->GetActorLocation().X)
		{
			// Set the next platform
			PlatformToCheck = PlatformToCheck->GetNextPlatform();

			if (IsValid(FirstPlatform))
			{
				// Add the platform's length to walked distance, in meters
				PlayerController->AddToCurrentDistance(FirstPlatform->GetPlatformLength() / 1000);

				// Destroy first platform and set the next first platform
				APlatformModule* TempFirstPlatform = FirstPlatform->GetNextPlatform();
				PlatformsContainerActor->RemovePlatformModule(FirstPlatform);
				FirstPlatform->DestroyPlatform();

				FirstPlatform = TempFirstPlatform;
			}

			PlayerController->SetCurrentPlatform(PlatformToCheck);

			// Pickup Rush Mechanic
			APlatformModule* TempPlatform = FirstPlatform;
			for (int PlatformIndex = 0; PlatformIndex < PickupsRushPlatformsCount; PlatformIndex++)
			{
				TempPlatform->StartPickupRush();

				TempPlatform = FirstPlatform->GetNextPlatform();
				if (!IsValid(TempPlatform)) break;
			}

			// Obstacles Rush Mechanic
			TempPlatform = FirstPlatform;
			for (int PlatformIndex = 0; PlatformIndex < ObstaclesRushPlatformsCount; PlatformIndex++)
			{
				TempPlatform->StartObstacleRush();

				TempPlatform = FirstPlatform->GetNextPlatform();
				if (!IsValid(TempPlatform)) break;
			}

			// Spawn new Platform
			SpawnNewPlatform();
		}
	}
}

// Initialize platforms and player
void AEndlessCleanerGameMode_Level::InitializeGame()
{
	PreviousPlatform = nullptr;

	PlatformCount = 0;

	if (!IsValid(PlatformsContainerActor))
		PlatformsContainerActor = Cast<APlatformsContainer>(GetWorld()->SpawnActor(PlatformsContainerClass));

	FVector SpawnPosition = bInitializeFromPlayerStart ? FindPlayerStart(PlayerController)->GetActorLocation() : FVector::ZeroVector;

	for (int PlatformIndex = 0; PlatformIndex < VisiblePlatformNumber; PlatformIndex++)
	{
		APlatformModule* SpawnedPlatform = nullptr;

		// Set the initial platforms as a normal ground
		if (PlatformCount <= NumberOfInitialPlatforms)
		{
			TArray<EPlatformType> TypesFilter =
			{
				EPlatformType::VE_Ground
			};

			TSubclassOf<APlatformModule> PlatformToSpawn = GetPlatformToSpawn(TypesFilter);
			SpawnedPlatform = GetWorld()->SpawnActor<APlatformModule>(PlatformToSpawn, SpawnPosition, FRotator::ZeroRotator);
		}
		else
		{
			// Next platforms, spawn according to probability
			if (IsValid(PreviousPlatform))
			{
				TSubclassOf<APlatformModule> PlatformToSpawn;

				// Next type to spawn: Depending on previous platform spawned
				if (!bUseNormalGroundBetweenBridges ||
					PreviousPlatform->GetPlatformType() == EPlatformType::VE_Ground ||
					PreviousPlatform->GetPlatformType() == EPlatformType::VE_GroundGap)
				{
					PlatformToSpawn = GetPlatformToSpawn();
				}
				else
				{
					TArray<EPlatformType> TypesFilter =
					{
						EPlatformType::VE_Ground,
						EPlatformType::VE_GroundGap
					};

					PlatformToSpawn = GetPlatformToSpawn(TypesFilter);
				}

				if (IsValid(PlatformToSpawn))
				{
					SpawnedPlatform = GetWorld()->SpawnActor<APlatformModule>(PlatformToSpawn, SpawnPosition, FRotator::ZeroRotator);
				}
			}
		}

		// Spawn the selected Platform
		if (IsValid(SpawnedPlatform))
		{
			SpawnedPlatform->AttachToActor(PlatformsContainerActor, FAttachmentTransformRules::KeepWorldTransform);
			PlatformsContainerActor->AddPlatformModule(SpawnedPlatform);
			SpawnedPlatform->SetActorLocation(SpawnPosition);
			SpawnedPlatform->SetActorRotation(PlatformsContainerActor->GetActorRotation());
			SpawnPosition.X += SpawnedPlatform->GetPlatformLength();

			// Set Previous and Next Platforms
			if (IsValid(PreviousPlatform))
			{
				SpawnedPlatform->SetPreviousPlatform(PreviousPlatform);
				PreviousPlatform->SetNextPlatform(SpawnedPlatform);

				// Dont spawn for the first few platform
				if (PlatformCount >= NumberOfInitialPlatforms)
				{
					PreviousPlatform->SpawnObstacles();
					PreviousPlatform->SpawnPickups();
				}
			}
		}

		// Set the previous platform as the spawned platform
		PreviousPlatform = SpawnedPlatform;

		// Place the platform at the beginning of the second platform
		if (PlatformCount == 1)
		{
			PlatformToCheck = SpawnedPlatform;

			FVector PlatformToCheckPosition = PlatformToCheck->GetActorLocation();
			Player->SetActorLocation(PlatformToCheckPosition);
		}

		// Platform to remove
		if (PlatformCount == 0)
		{
			FirstPlatform = SpawnedPlatform;
		}

		PlatformCount++;
	}

	// Create Enemy Factory and spawn Enemy
	SpawnEnemy();
}

void AEndlessCleanerGameMode_Level::OnRespawn()
{
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

	if (GameState == EGameState::VE_PrepareGame)
	{
		if (IsValid(PlayerController->GetUI()))
		{
			PlayerController->GetUI()->OnStartGame();

			GameState = EGameState::VE_Respawn;

			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AEndlessCleanerGameMode_Level::OnRespawn, SpawnStartTimer, false);
		}
	}

	else if (GameState == EGameState::VE_RemovePlatforms)
	{
		// Reset container rotation
		PlatformsContainerActor->ResetRotation();

		// Remove platforms
		APlatformModule* FirstIterate = FirstPlatform;

		while (FirstIterate->GetNextPlatform())
		{
			APlatformModule* Temp = FirstIterate->GetNextPlatform();
			PlatformsContainerActor->RemovePlatformModule(FirstIterate);
			FirstIterate->DestroyPlatform();
			FirstIterate = Temp;
		}

		if (IsValid(FirstIterate))
		{
			PlatformsContainerActor->RemovePlatformModule(FirstIterate);
			FirstIterate->DestroyPlatform();
		}

		FirstPlatform = nullptr;

		// Initialize game
		InitializeGame();

		GameState = EGameState::VE_Respawn;

		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AEndlessCleanerGameMode_Level::OnRespawn, RespawnTimer, false);
	}

	else if (GameState == EGameState::VE_Respawn)
	{
		AAudioManager::GetInstance()->PlayBackgroundMusic();

		PlayerController->Respawn();
		PlayerController->StartRunning();
		GameState = EGameState::VE_Running;
	}
}

void AEndlessCleanerGameMode_Level::SpawnNewPlatform()
{
	if (!IsValid(PreviousPlatform)) return;

	FVector SpawnPosition = PreviousPlatform->GetActorLocation();
	SpawnPosition.X += PreviousPlatform->GetPlatformLength();

	APlatformModule* SpawnedPlatform = nullptr;

	// Next type to spawn: Depending on previous platform spawned
	TSubclassOf<APlatformModule> PlatformToSpawn;
	if (!bUseNormalGroundBetweenBridges ||
		PreviousPlatform->GetPlatformType() == EPlatformType::VE_Ground ||
		PreviousPlatform->GetPlatformType() == EPlatformType::VE_GroundGap)
	{
		PlatformToSpawn = GetPlatformToSpawn();
	}
	else
	{
		TArray<EPlatformType> TypesFilter =
		{
			EPlatformType::VE_Ground,
			EPlatformType::VE_GroundGap
		};

		PlatformToSpawn = GetPlatformToSpawn(TypesFilter);
	}

	// Spawn the selected Platform
	if (IsValid(PlatformToSpawn))
	{
		SpawnedPlatform = GetWorld()->SpawnActor<APlatformModule>(PlatformToSpawn, SpawnPosition, FRotator::ZeroRotator);

		SpawnedPlatform->AttachToActor(PlatformsContainerActor, FAttachmentTransformRules::KeepWorldTransform);
		PlatformsContainerActor->AddPlatformModule(SpawnedPlatform);
		SpawnedPlatform->SetActorLocation(SpawnPosition);
		SpawnedPlatform->SetActorRotation(PlatformsContainerActor->GetActorRotation());
		SpawnPosition.X += SpawnedPlatform->GetPlatformLength();

		// Set Previous and Next Platforms
		if (IsValid(PreviousPlatform))
		{
			SpawnedPlatform->SetPreviousPlatform(PreviousPlatform);
			PreviousPlatform->SetNextPlatform(SpawnedPlatform);

			// Dont spawn for the first few platform
			if (PlatformCount >= NumberOfInitialPlatforms)
			{
				PreviousPlatform->SpawnObstacles();
				PreviousPlatform->SpawnPickups();
			}
		}

		// Set the previous platform as the spawned platform
		PreviousPlatform = SpawnedPlatform;

		PlatformCount++;
	}

	// Add to enemies' waypoint
	if (IsValid(SpawnedPlatform) && EnemyFactoryRef)
	{
		TArray<AEnemy*> EnemyList = EnemyFactoryRef->GetEnemyList();
		for (auto& Enemy : EnemyList)
		{
			if (IsValid(Enemy))
				Enemy->AddWaypoint(SpawnedPlatform->GetActorLocation());
		}
	}
}

TSubclassOf<APlatformModule> AEndlessCleanerGameMode_Level::GetPlatformToSpawn(TArray<EPlatformType> PlatformTypeFilters)
{
	TArray<FPlatformOptions> ProbabilityTable;

	for (int i = 0; i < PlatformTable.Num(); i++)
	{
		if (PlatformTypeFilters.Num() <= 0 ||
			PlatformTypeFilters.Contains(PlatformTable[i].PlatformType))
			ProbabilityTable.Add(PlatformTable[i]);
	}

	// Loop through all probabilities
	float TotalProbabilities = 0;
	for (int i = 0; i < ProbabilityTable.Num(); i++)
	{
		TotalProbabilities += ProbabilityTable[i].Probability;
	}

	// Get a random point 
	float RandomPoint = FMath::FRand() * TotalProbabilities;
	int PlatformIndex = -1;

	for (int i = 0; i < ProbabilityTable.Num(); i++)
	{
		if (RandomPoint < ProbabilityTable[i].Probability)
		{
			PlatformIndex = i;
			break;
		}
		else
		{
			RandomPoint -= ProbabilityTable[i].Probability;
		}
	}

	// If no index was found
	if (PlatformIndex == -1)
	{
		PlatformIndex = ProbabilityTable.Num() - 1;
	}

	// Return the selected platform
	return	ProbabilityTable[PlatformIndex].PlatformClass;
}

void AEndlessCleanerGameMode_Level::SpawnEnemy()
{
	if (!EnemyFactoryRef)
		EnemyFactoryRef = new EnemyFactory(GetWorld());

	if (IsValid(EnemyClass) && IsValid(FirstPlatform))
	{
		AEnemy* SpawnedEnemy = EnemyFactoryRef->CreateEnemy(EnemyClass, FirstPlatform->GetActorLocation());

		// Add all existing waypoints
		for (APlatformModule* TempModule = FirstPlatform; IsValid(TempModule->GetNextPlatform()); TempModule = TempModule->GetNextPlatform())
			SpawnedEnemy->AddWaypoint(TempModule->GetActorLocation());
	}
}

void AEndlessCleanerGameMode_Level::OnEnemyKilled(AEnemy* KilledEnemy)
{
	if (EnemyFactoryRef) EnemyFactoryRef->DestroyEnemy(KilledEnemy);
	SpawnEnemy();
}

void AEndlessCleanerGameMode_Level::OnTriggerDeathActor()
{
	if (GameState == EGameState::VE_GameOver)
		return;

	// Set Game over condition when player hits the ground
	PlayerController->StopRunning();
	PlatformsContainerActor->StopRotation();

	bool bIsLastLife = false;
	PlayerController->LoseLife(bIsLastLife);

	if (bIsLastLife)
	{
		GameState = EGameState::VE_GameOver;

		// Score Calculate Score
		float Score = 0;
		Score += PlayerController->GetCoinsCollected() * CoinsMultiplier;
		Score += (PlayerController->GetCurrentDistance() / PlayerController->GetCurrentTime()) * SpeedMultiplier;

		if (IsValid(PlayerController->GetUI()))
		{
			PlayerController->GetUI()->SetScore(Score);
			PlayerController->GetUI()->OnGameOver();
		}
	}
	else
	{
		GameState = EGameState::VE_RemovePlatforms;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AEndlessCleanerGameMode_Level::OnRespawn, RespawnTimer, false);
	}
}

#pragma region Debug
void AEndlessCleanerGameMode_Level::PrintDebugLog(FString Log, FColor Color)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Log);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, Color, Log);
}

void AEndlessCleanerGameMode_Level::SetDebugLogState(bool State)
{
	FString Command = "Log LogTemp ";

	if (State)
		Command += "Log";
	else
		Command += "Off";

	GEngine->Exec(nullptr, *Command);
}

void AEndlessCleanerGameMode_Level::SetDebugScreenLogState(bool State)
{
	GEngine->bEnableOnScreenDebugMessages = State;
}
#pragma endregion