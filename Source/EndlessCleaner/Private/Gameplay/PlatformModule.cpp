// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlatformModule.h"
#include "Gameplay/Pickup.h"

// Sets default values
APlatformModule::APlatformModule()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup initial variables
	PlatformLength = 20.0f;
	DistanceBetweenLanes = 150.0f;
	PickupStartLocationX = -170.0f;
	DistanceBetweenPickupsX = 230.0f;
	PickupsNumberPerSpawn = 3;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StartModulePoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartModulePoint"));
	StartModulePoint->SetupAttachment(RootComponent);

	EndModulePoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndModulePoint"));
	EndModulePoint->SetupAttachment(RootComponent);

	// Setup Spawn Pickup Positions
	Lane0 = CreateDefaultSubobject<USceneComponent>(TEXT("Lane0"));
	Lane0->SetupAttachment(RootComponent);

	Lane1 = CreateDefaultSubobject<USceneComponent>(TEXT("Lane1"));
	Lane1->SetupAttachment(RootComponent);

	Lane2 = CreateDefaultSubobject<USceneComponent>(TEXT("Lane2"));
	Lane2->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlatformModule::BeginPlay()
{
	Super::BeginPlay();

	PlatformLength = FMath::Abs(StartModulePoint->GetComponentLocation().X - EndModulePoint->GetComponentLocation().X);
}

// Dont spawn for VE_GroundGap
// Dont spawn for the first few platform
void APlatformModule::SpawnPickups(int PlatformCount)
{
	if (!PickupClass || PlatformCount < 5) return;

	int Lane = 1;
	bool bSpawnPickup = false;

	switch (PlatformGroundType)
	{
	case EPlatformGroundType::VE_Ground: // according to the next platform
		if (NextPlatform)
		{
			if (NextPlatform->GetPlatformType() == EPlatformGroundType::VE_CenterBridge)
			{
				bSpawnPickup = true;
				Lane = 1;
			}
			else if (NextPlatform->GetPlatformType() == EPlatformGroundType::VE_LeftBridge)
			{
				bSpawnPickup = true;
				Lane = 2;
			}
			else if (NextPlatform->GetPlatformType() == EPlatformGroundType::VE_RightBridge)
			{
				bSpawnPickup = true;
				Lane = 0;
			}
			else
			{
				bSpawnPickup = true;
				Lane = FMath::RandRange(0, 2);
			}
		}
		break;

	case EPlatformGroundType::VE_CenterBridge:
		bSpawnPickup = true;
		Lane = 1;
		break;


	case EPlatformGroundType::VE_LeftBridge:
		bSpawnPickup = true;
		Lane = 2;
		break;

	case EPlatformGroundType::VE_RightBridge:
		bSpawnPickup = true;
		Lane = 0;
		break;

	case EPlatformGroundType::VE_GroundGap:
		break;
	}

	if (bSpawnPickup)
	{
		FVector SpawnPosition = Lane0->GetComponentLocation();

		if (Lane == 1)
		{
			SpawnPosition = Lane1->GetComponentLocation();
		}
		else if (Lane == 2)
		{
			SpawnPosition = Lane2->GetComponentLocation();
		}

		// Spawn pickups
		for (int i = 0; i < PickupsNumberPerSpawn; i++)
		{
			APickup* SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClass, SpawnPosition, FRotator::ZeroRotator);

			if (SpawnedPickup)
			{
				SpawnedPickup->SetActorLocation(SpawnPosition);
				SpawnedPickups.Add(SpawnedPickup);

				SpawnPosition.X -= DistanceBetweenPickupsX;
			}
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