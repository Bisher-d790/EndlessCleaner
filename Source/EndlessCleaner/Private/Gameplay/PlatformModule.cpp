// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlatformModule.h"
#include "Gameplay/Pickup.h"
#include "Gameplay/Obstacle.h"

// Sets default values
APlatformModule::APlatformModule()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StartModulePoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartModulePoint"));
	StartModulePoint->SetupAttachment(RootComponent);

	EndModulePoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndModulePoint"));
	EndModulePoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlatformModule::BeginPlay()
{
	Super::BeginPlay();

	PlatformLength = FMath::Abs(StartModulePoint->GetComponentLocation().X - EndModulePoint->GetComponentLocation().X);
}

void APlatformModule::SpawnPickups()
{
	TArray<FLaneOptions> ProbabilityTable = Lanes;
	int LaneIndex = -1;
	bool bSpawnPickup = false;

	// Loop through all probabilities
	float TotalProbabilities = 0;
	for (int i = 0; i < ProbabilityTable.Num(); i++)
	{
		TotalProbabilities += ProbabilityTable[i].PickupProbability;
	}

	// Get a random point 
	float RandomPoint = FMath::FRand() * TotalProbabilities;

	for (int i = 0; i < ProbabilityTable.Num(); i++)
	{
		if (RandomPoint < ProbabilityTable[i].PickupProbability)
		{
			LaneIndex = i;
			bSpawnPickup = true;
			break;
		}
		else
		{
			RandomPoint -= ProbabilityTable[i].PickupProbability;
		}
	}

	// If no index was found
	if (LaneIndex == -1)
	{
		bSpawnPickup = false;
	}

	if (bSpawnPickup && Lanes[LaneIndex].PickupClass)
	{
		FVector SpawnPosition = GetActorLocation() + Lanes[LaneIndex].LanePosition;
		SpawnPosition += Lanes[LaneIndex].PickupStartPosition;

		// Spawn pickups
		for (int i = 0; i < Lanes[LaneIndex].PickupsNumberPerSpawn; i++)
		{
			APickup* SpawnedPickup = GetWorld()->SpawnActor<APickup>(Lanes[LaneIndex].PickupClass, SpawnPosition, FRotator::ZeroRotator);
			SpawnedPickup->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			SpawnedPickup->SetActorLocation(SpawnPosition);

			SpawnedPickups.Add(SpawnedPickup);

			SpawnPosition -= Lanes[LaneIndex].DistanceBetweenPickups;
		}
	}
}

void APlatformModule::SpawnObstacles()
{
	for (int i = 0; i < Lanes.Num(); i++)
	{
		if (FMath::FRand() * 100 < Lanes[i].ObstacleProbability && Lanes[i].ObstacleClass)
		{
			FVector SpawnPosition = GetActorLocation() + Lanes[i].LanePosition;
			SpawnPosition += Lanes[i].ObstaclePosition;

			AObstacle* SpawnedObstacle = GetWorld()->SpawnActor<AObstacle>(Lanes[i].ObstacleClass, SpawnPosition, FRotator::ZeroRotator);
			SpawnedObstacle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			SpawnedObstacle->SetActorLocation(SpawnPosition);

			SpawnedObstacles.Add(SpawnedObstacle);
		}
	}
}

// Called when the platform is destroyed
void APlatformModule::DestroyPlatform()
{
	//UE_LOG(LogTemp, Warning, TEXT("Destroying platform"));

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PreviousPlatform = nullptr;
	NextPlatform = nullptr;

	for (int i = 0; i < SpawnedPickups.Num(); i++)
	{
		SpawnedPickups[i]->Destroy();
	}

	for (int i = 0; i < SpawnedObstacles.Num(); i++)
	{
		SpawnedObstacles[i]->Destroy();
	}

	Destroy();
}