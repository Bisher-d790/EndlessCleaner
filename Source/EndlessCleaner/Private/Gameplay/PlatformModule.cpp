// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlatformModule.h"
#include "Gameplay/Pickup.h"

// Sets default values
APlatformModule::APlatformModule()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup initial variables
	DistanceBetweenLanes = 150.0f;

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
		SpawnPosition.X += Lanes[LaneIndex].PickupStartLocationX;

		// Spawn pickups
		for (int i = 0; i < Lanes[LaneIndex].PickupsNumberPerSpawn; i++)
		{
			APickup* SpawnedPickup = GetWorld()->SpawnActor<APickup>(Lanes[LaneIndex].PickupClass, SpawnPosition, FRotator::ZeroRotator);
			SpawnedPickup->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			SpawnedPickup->SetActorLocation(SpawnPosition);

			SpawnedPickups.Add(SpawnedPickup);

			SpawnPosition.X -= Lanes[LaneIndex].DistanceBetweenPickupsX;
		}
	}
}

// Called when the platform is destroyed
void APlatformModule::DestroyPlatform()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PreviousPlatform = nullptr;
	NextPlatform = nullptr;

	for (int i = SpawnedPickups.Num() - 1; i >= 0; i++)
	{
		SpawnedPickups[i]->Destroy();
	}

	Destroy();
}