// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/EndlessCleanerGameController.h"
#include "Player/EndlessCleanerPlayerController.h"
#include "Player/EndlessCleanerCharacter.h"
#include "Gameplay/PlatformModule.h"
#include "UI/InGameUIWidget.h"
#include "Utils/SingletonManager.h"


#pragma region Singleton
AEndlessCleanerGameController* AEndlessCleanerGameController::GetInstance()
{
	if (GEngine)
	{
		USingletonManager* SingletonManager = Cast<USingletonManager>(GEngine->GameSingleton);
		return SingletonManager->GetGameControllerInstance();
	}
	return nullptr;
}

void AEndlessCleanerGameController::SetInstance()
{
	if (GEngine)
	{
		USingletonManager* SingletonManager = Cast<USingletonManager>(GEngine->GameSingleton);
		SingletonManager->SetGameControllerInstance(this);
	}
}
#pragma endregion Singleton

// Sets default values
AEndlessCleanerGameController::AEndlessCleanerGameController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Variables values
	NumberOfInitialPlatforms = 5;
	VisiblePlatformNumber = 9;
	RespawnTimer = 3.0f;
	SpawnStartTimer = 2.0f;
	InitialPlayerLives = 3;

	LivesMultiplier = 1000;
	CoinsMultiplier = 100;
	SpeedMultiplier = 1000;
}

// Called when the game starts or when spawned
void AEndlessCleanerGameController::BeginPlay()
{
	Super::BeginPlay();

	SetInstance();

	// Set Initial Game State
	GameState = EGameState::VE_PreparePlatforms;

	PlayerController = Cast<AEndlessCleanerPlayerController>(GetWorld()->GetFirstPlayerController());

	Player = Cast<AEndlessCleanerCharacter>(PlayerController->GetCharacter());

	InitializeGame();

	PlayerController->SetInitialLives(InitialPlayerLives);

	GameState = EGameState::VE_PrepareGame;

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AEndlessCleanerGameController::OnRespawn, 0.1f, false);
}

// Called every frame
void AEndlessCleanerGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player || GameState != EGameState::VE_Running) return;

	// Move the environmental actors with the player
	if (EnvironmentalActorsToMove.Num() > 0)
	{
		for (int i = 0; i < EnvironmentalActorsToMove.Num(); i++)
		{
			FVector ActorPosition = EnvironmentalActorsToMove[i]->GetActorLocation();
			ActorPosition.X = Player->GetActorLocation().X;

			EnvironmentalActorsToMove[i]->SetActorLocation(ActorPosition);
		}
	}

	if (PlatformToCheck)
	{
		if (Player->GetActorLocation().X > PlatformToCheck->GetActorLocation().X)
		{
			// Set the next platform
			PlatformToCheck = PlatformToCheck->GetNextPlatform();

			if (FirstPlatform)
			{
				// Add the platform's length to walked distance, in meters
				PlayerController->AddToCurrentDistance(FirstPlatform->GetPlatformLength() / 1000);

				// Destroy first platform and set the next first platform
				APlatformModule* TempFirstPlatform = FirstPlatform->GetNextPlatform();
				FirstPlatform->DestroyPlatform();

				FirstPlatform = TempFirstPlatform;
			}

			PlayerController->SetCurrentPlatform(PlatformToCheck);

			// Spawn new Platform
			SpawnNewPlatform();
		}
	}

	// Set Game over condition when player hits the ground
	if (DeathGround)
	{
		if (Player->GetActorLocation().Z <= DeathGround->GetActorLocation().Z)
		{
			PlayerController->StopRunning();

			bool bIsLastLife = false;
			PlayerController->LoseLife(bIsLastLife);

			if (bIsLastLife)
			{
				GameState = EGameState::VE_GameOver;

				// Score Calculate Score
				float Score = 0;
				Score += PlayerController->GetCurrentLives() * LivesMultiplier;
				Score += PlayerController->GetCoinsCollected() * CoinsMultiplier;
				Score += (PlayerController->GetCurrentDistance() / PlayerController->GetCurrentTime()) * SpeedMultiplier;

				if (PlayerController->GetUI())
				{
					PlayerController->GetUI()->SetScore(Score);
					PlayerController->GetUI()->OnGameOver();
				}
			}
			else
			{
				GameState = EGameState::VE_RemovePlatforms;
				GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AEndlessCleanerGameController::OnRespawn, RespawnTimer, false);
			}
		}
	}
}

// Initialize platforms and player
void AEndlessCleanerGameController::InitializeGame()
{
	PreviousPlatform = nullptr;

	PlatformCount = 0;

	FVector SpawnPosition = GetActorLocation();

	for (int PlatformIndex = 0; PlatformIndex < VisiblePlatformNumber; PlatformIndex++)
	{
		APlatformModule* SpawnedPlatform = nullptr;

		// Set the initial platforms as a normal ground
		if (PlatformCount <= NumberOfInitialPlatforms)
		{
			TArray<EPlatformType> TypesFilter =
			{
				EPlatformType::VE_ThreeLanes_Ground
			};

			TSubclassOf<APlatformModule> PlatformToSpawn = GetPlatformToSpawn(TypesFilter);
			SpawnedPlatform = GetWorld()->SpawnActor<APlatformModule>(PlatformToSpawn, SpawnPosition, FRotator::ZeroRotator);
		}
		else
		{
			// Next platforms, spawn according to probability
			if (PreviousPlatform)
			{
				TSubclassOf<APlatformModule> PlatformToSpawn;

				// Next type to spawn: Depending on previous platform spawned
				if (!bUseNormalGroundBetweenBridges ||
					PreviousPlatform->GetPlatformType() == EPlatformType::VE_ThreeLanes_Ground ||
					PreviousPlatform->GetPlatformType() == EPlatformType::VE_ThreeLanes_GroundGap)
				{
					PlatformToSpawn = GetPlatformToSpawn();
				}
				else
				{
					TArray<EPlatformType> TypesFilter =
					{
						EPlatformType::VE_ThreeLanes_Ground,
						EPlatformType::VE_ThreeLanes_GroundGap
					};

					PlatformToSpawn = GetPlatformToSpawn(TypesFilter);
				}

				if (PlatformToSpawn)
				{
					SpawnedPlatform = GetWorld()->SpawnActor<APlatformModule>(PlatformToSpawn, SpawnPosition, FRotator::ZeroRotator);
				}
			}
		}

		// Spawn the selected Platform
		if (SpawnedPlatform)
		{
			SpawnedPlatform->AttachToActor(PlatformsParentActor, FAttachmentTransformRules::KeepWorldTransform);
			SpawnedPlatform->SetActorLocation(SpawnPosition);
			SpawnPosition.X += SpawnedPlatform->GetPlatformLength();

			// Set Previous and Next Platforms
			if (PreviousPlatform)
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
}

void AEndlessCleanerGameController::OnRespawn()
{
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

	if (GameState == EGameState::VE_PrepareGame)
	{
		if (PlayerController->GetUI())
		{
			PlayerController->GetUI()->OnStartGame();

			GameState = EGameState::VE_Respawn;

			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AEndlessCleanerGameController::OnRespawn, SpawnStartTimer, false);
		}
	}

	else if (GameState == EGameState::VE_RemovePlatforms)
	{
		// Remove platforms
		APlatformModule* FirstIterate = FirstPlatform;

		while (FirstIterate->GetNextPlatform())
		{
			APlatformModule* Temp = FirstIterate->GetNextPlatform();
			FirstIterate->DestroyPlatform();
			FirstIterate = Temp;
		}

		if (FirstIterate)
		{
			FirstIterate->DestroyPlatform();
		}

		FirstPlatform = nullptr;

		// Initialize game
		InitializeGame();

		GameState = EGameState::VE_Respawn;

		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AEndlessCleanerGameController::OnRespawn, RespawnTimer, false);
	}

	else if (GameState == EGameState::VE_Respawn)
	{
		PlayerController->Respawn();
		PlayerController->StartRunning();
		GameState = EGameState::VE_Running;
	}
}

void AEndlessCleanerGameController::SpawnNewPlatform()
{
	if (!PreviousPlatform) return;

	FVector SpawnPosition = PreviousPlatform->GetActorLocation();
	SpawnPosition.X += PreviousPlatform->GetPlatformLength();

	// Next type to spawn: Depending on previous platform spawned
	TSubclassOf<APlatformModule> PlatformToSpawn;
	if (!bUseNormalGroundBetweenBridges ||
		PreviousPlatform->GetPlatformType() == EPlatformType::VE_ThreeLanes_Ground ||
		PreviousPlatform->GetPlatformType() == EPlatformType::VE_ThreeLanes_GroundGap)
	{
		PlatformToSpawn = GetPlatformToSpawn();
	}
	else
	{
		TArray<EPlatformType> TypesFilter =
		{
			EPlatformType::VE_ThreeLanes_Ground,
			EPlatformType::VE_ThreeLanes_GroundGap
		};

		PlatformToSpawn = GetPlatformToSpawn(TypesFilter);
	}

	// Spawn the selected Platform
	if (PlatformToSpawn)
	{
		APlatformModule* SpawnedPlatform = GetWorld()->SpawnActor<APlatformModule>(PlatformToSpawn, SpawnPosition, FRotator::ZeroRotator);

		SpawnedPlatform->AttachToActor(PlatformsParentActor, FAttachmentTransformRules::KeepWorldTransform);
		SpawnedPlatform->SetActorLocation(SpawnPosition);
		SpawnPosition.X += SpawnedPlatform->GetPlatformLength();

		// Set Previous and Next Platforms
		if (PreviousPlatform)
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
}

TSubclassOf<class APlatformModule> AEndlessCleanerGameController::GetPlatformToSpawn(TArray<EPlatformType> PlatformTypeFilters)
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