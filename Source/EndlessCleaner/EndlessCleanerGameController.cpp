// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessCleanerGameController.h"
#include "EndlessCleanerPlayerController.h"
#include "EndlessCleanerCharacter.h"
#include "PlatformModule.h"

// Sets default values
AEndlessCleanerGameController::AEndlessCleanerGameController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Variables values
	NumberOfInitialPlatforms = 5;

	VisiblePlatformNumber = 9;

	RespawnTimer = 3.0f;
}

// Called when the game starts or when spawned
void AEndlessCleanerGameController::BeginPlay()
{
	Super::BeginPlay();

	GameState = EGameState::VE_PreparePlatforms;

	PlayerController = Cast<AEndlessCleanerPlayerController>(GetWorld()->GetFirstPlayerController());

	Player = Cast<AEndlessCleanerCharacter>(PlayerController->GetCharacter());

	InitializeGame();

	GameState = EGameState::VE_PrepareGame;

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AEndlessCleanerGameController::OnRespawn, RespawnTimer, false);
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

			// Spawn new Platform
			SpawnNewPlatform();
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
		// afterwards Find platform by type, using randomly generated numbers
		int32 RandomIndex;
		if (PlatformIndex <= NumberOfInitialPlatforms)
		{
			RandomIndex = (int32)EPlatformGroundType::VE_Ground;
		}
		else
		{
			RandomIndex = FMath::RandRange(0, PlatformTypesRandomCeil);
		}

		for (int i = 0; i < PlatformTable.Num(); i++)
		{
			if (RandomIndex == (int32)PlatformTable[i].PlatformGroundType)
			{
				SpawnedPlatform = GetWorld()->SpawnActor<APlatformModule>(PlatformTable[i].PlatformClass, SpawnPosition, FRotator::ZeroRotator);
				break;
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

void AEndlessCleanerGameController::OnRespawn() {
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

	PlayerController->Respawn();
	PlayerController->StartRunning();

	GameState = EGameState::VE_Running;
}

void AEndlessCleanerGameController::SpawnNewPlatform()
{
	if (!PreviousPlatform) return;

	FVector SpawnPosition = PreviousPlatform->GetActorLocation();
	SpawnPosition.X += PreviousPlatform->GetPlatformLength();

	TSubclassOf<APlatformModule> PlatformToSpawn = nullptr;

	// Find platform by type, using randomly generated numbers
	int32 RandomIndex = FMath::RandRange(0, PlatformTypesRandomCeil);
	for (int i = 0; i < PlatformTable.Num(); i++)
	{
		if (RandomIndex == (int32)PlatformTable[i].PlatformGroundType)
		{
			PlatformToSpawn = PlatformTable[i].PlatformClass;
			break;
		}
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
		}

		// Set the previous platform as the spawned platform
		PreviousPlatform = SpawnedPlatform;

		PlatformCount++;
	}
}
