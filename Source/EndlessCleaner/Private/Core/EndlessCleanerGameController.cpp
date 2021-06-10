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
	PlayerLives = 3;
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

	PlayerController->SetInitialLives(PlayerLives);

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
				// Destroy first platform and set the next first platform
				APlatformModule* TempFirstPlatform = FirstPlatform->GetNextPlatform();
				FirstPlatform->Destroy();

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

				if (PlayerController->GetUI())
				{
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
			float RandomPlatformToSpawn = FMath::RandRange(0.f, 100.f);

			EPlatformType PlatformType = EPlatformType::VE_Ground;

			// Select normal ground
			if (RandomPlatformToSpawn > 50.f)
			{
				PlatformType = EPlatformType::VE_Ground;
			}

			TSubclassOf<APlatformModule> PlatformToSpawn = GetPlatformModuleByType(PlatformType);
			SpawnedPlatform = GetWorld()->SpawnActor<APlatformModule>(PlatformToSpawn, SpawnPosition, FRotator::ZeroRotator);
		}
		else
		{
			// Next platforms, spawn according to probability
			if (PreviousPlatform)
			{
				// Next type to spawn: Depending on previous platform spawned
				EPlatformType NewType = GetPlatformTypeToSpawn(PreviousPlatform->GetPlatformType());
				TSubclassOf<APlatformModule> PlatformToSpawn = GetPlatformModuleByType(NewType);

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
			PlatformToCheckPosition.Z = 280.0f;
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
			FirstIterate->Destroy();
			FirstIterate = Temp;
		}

		if (FirstIterate)
		{
			FirstIterate->Destroy();
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

	EPlatformType NewType = GetPlatformTypeToSpawn(PreviousPlatform->GetPlatformType());
	TSubclassOf<APlatformModule> PlatformToSpawn = GetPlatformModuleByType(NewType);

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
				PreviousPlatform->SpawnPickups();
			}
		}

		// Set the previous platform as the spawned platform
		PreviousPlatform = SpawnedPlatform;

		PlatformCount++;
	}
}

EPlatformType AEndlessCleanerGameController::GetPlatformTypeToSpawn(const EPlatformType& PlatformType)
{
	EPlatformType SelectedPlatform = EPlatformType::VE_Ground;
	TArray<FProbabilityTable> TempTable;

	if (PlatformType == EPlatformType::VE_Ground ||
		PlatformType == EPlatformType::VE_GroundGap)
	{
		for (int i = 0; i < PlatformTable.Num(); i++)
		{
			TempTable.Add(PlatformTable[i]);
		}
	}
	else if (PlatformType == EPlatformType::VE_CenterBridge ||
		PlatformType == EPlatformType::VE_LeftBridge ||
		PlatformType == EPlatformType::VE_RightBridge)
	{
		for (int i = 0; i < PlatformTable.Num(); i++)
		{
			if (PlatformTable[i].PlatformType == EPlatformType::VE_Ground ||
				PlatformTable[i].PlatformType == EPlatformType::VE_GroundGap)
			{
				TempTable.Add(PlatformTable[i]);
			}
		}
	}

	if (TempTable.Num() == 0)
	{
		return EPlatformType::VE_Ground;
	}

	// Loop through all probabilities
	float Total = 0;
	for (int i = 0; i < TempTable.Num(); i++)
	{
		Total += TempTable[i].Probability;
	}

	// Get a random point 
	float RandomPoint = FMath::FRand() * Total;
	int PlatformIndex = -1;

	for (int i = 0; i < TempTable.Num(); i++)
	{
		if (RandomPoint < TempTable[i].Probability)
		{
			PlatformIndex = i;
			break;
		}
		else
		{
			RandomPoint -= TempTable[i].Probability;
		}
	}

	// If no index was found
	if (PlatformIndex == -1)
	{
		PlatformIndex = TempTable.Num() - 1;
	}

	// Select the platform
	if (PlatformIndex < TempTable.Num())
	{
		SelectedPlatform = TempTable[PlatformIndex].PlatformType;
	}

	return SelectedPlatform;
}

TSubclassOf<class APlatformModule> AEndlessCleanerGameController::GetPlatformModuleByType(const EPlatformType& PreviousPlatformType)
{
	for (int i = 0; i < PlatformTable.Num(); i++)
	{
		if (PreviousPlatformType == PlatformTable[i].PlatformType)
		{
			return PlatformTable[i].PlatformClass;
		}
	}

	return nullptr;
}