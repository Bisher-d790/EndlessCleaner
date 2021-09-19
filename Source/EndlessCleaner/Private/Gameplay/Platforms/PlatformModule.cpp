// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Platforms/PlatformModule.h" 
#include "Gameplay/Pickup.h"
#include "Gameplay/Obstacle.h"
#include "Gameplay/Components/WaypointMovementComponent.h"
#include "Core/EndlessCleanerGameMode_Level.h"


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
		for (int PickupIndex = 0; PickupIndex < Lanes[LaneIndex].PickupsNumberPerSpawn; PickupIndex++)
		{
			APickup* SpawnedPickup = GetWorld()->SpawnActor<APickup>(Lanes[LaneIndex].PickupClass, SpawnPosition, FRotator::ZeroRotator);
			SpawnedPickup->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			// Set the initial location
			SpawnedPickup->SetActorLocation(SpawnPosition);

			// The waypoint component will controll the movement locally(the spawn position is the pivot point) 
			SpawnedPickup->WaypointMovementComponent->bMovementInLocalSpace = true;
			SpawnedPickup->WaypointMovementComponent->MovementSpeed = Lanes[LaneIndex].PickupsMovementSpeed;
			for (int WaypointIndex = 0; WaypointIndex < Lanes[LaneIndex].PickupLocalWaypoints.Num(); WaypointIndex++)
			{
				FVector WaypointLocation = FVector(
					SpawnPosition.X + Lanes[LaneIndex].PickupLocalWaypoints[WaypointIndex].X,
					SpawnPosition.Y + Lanes[LaneIndex].PickupLocalWaypoints[WaypointIndex].Y,
					SpawnPosition.Z + Lanes[LaneIndex].PickupLocalWaypoints[WaypointIndex].Z);

				SpawnedPickup->WaypointMovementComponent->WaypointLocations.Add(WaypointLocation);
			}

			SpawnedPickups.Add(LaneIndex, SpawnedPickup);

			SpawnPosition.X -= Lanes[LaneIndex].DistanceBetweenPickups;
		}
	}
}

void APlatformModule::SpawnObstacles()
{
	for (int LaneIndex = 0; LaneIndex < Lanes.Num(); LaneIndex++)
	{
		if (FMath::FRand() * 100 < Lanes[LaneIndex].ObstacleProbability && Lanes[LaneIndex].ObstacleClass)
		{
			FVector SpawnPosition = GetActorLocation() + Lanes[LaneIndex].LanePosition;
			SpawnPosition += Lanes[LaneIndex].ObstaclePosition;

			AObstacle* SpawnedObstacle = GetWorld()->SpawnActor<AObstacle>(Lanes[LaneIndex].ObstacleClass, SpawnPosition, FRotator::ZeroRotator);
			SpawnedObstacle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			SpawnedObstacle->SetActorLocation(SpawnPosition);

			SpawnedObstacles.Add(LaneIndex, SpawnedObstacle);
		}
	}
}

// Called when the platform is destroyed
void APlatformModule::DestroyPlatform()
{
	AEndlessCleanerGameMode_Level::PrintDebugLog(TEXT("Destroying platform"));

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PreviousPlatform = nullptr;
	NextPlatform = nullptr;

	for (auto& Pickup : SpawnedPickups)
	{
		Pickup.Value->Destroy();
	}

	for (auto& Obstacle : SpawnedObstacles)
	{
		Obstacle.Value->Destroy();
	}

	Destroy();
}