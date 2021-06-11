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

// Dont spawn for VE_GroundGap
void APlatformModule::SpawnPickups()
{
	int Lane = 1;
	bool bSpawnPickup = false;

	switch (PlatformType)
	{
	case EPlatformType::VE_ThreeLanes_Ground: // according to the next platform
		if (NextPlatform)
		{
			bSpawnPickup = true;
			Lane = FMath::RandRange(0, 2);
		}
		break;

	case EPlatformType::VE_ThreeLanes_OneBridge:
	case EPlatformType::VE_ThreeLanes_TwoBridges:
		bSpawnPickup = true;
		Lane = FMath::RandRange(0, 2);
		break;

	case EPlatformType::VE_ThreeLanes_GroundGap:
		break;
	}

	if (bSpawnPickup && Lanes[Lane].PickupClass)
	{
		FVector SpawnPosition = GetActorLocation() + Lanes[Lane].LanePosition;

		// Spawn pickups
		for (int i = 0; i < Lanes[Lane].PickupsNumberPerSpawn; i++)
		{
			APickup* SpawnedPickup = GetWorld()->SpawnActor<APickup>(Lanes[Lane].PickupClass, SpawnPosition, FRotator::ZeroRotator);
			SpawnedPickup->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			if (SpawnedPickup)

				SpawnedPickup->SetActorLocation(SpawnPosition);
			SpawnedPickups.Add(SpawnedPickup);

			SpawnPosition.X -= Lanes[Lane].DistanceBetweenPickupsX;
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