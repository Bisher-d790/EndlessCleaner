// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Pickup.h"
#include "Gameplay/Components/WaypointMovementComponent.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	WaypointMovementComponent = CreateDefaultSubobject<UWaypointMovementComponent>(TEXT("Waypoint Movement"));
	AddInstanceComponent(WaypointMovementComponent);

	MovementStartDelayRandomMin = 0.f;
	MovementStartDelayRandomMax = 1.f;
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	WaypointMovementComponent->StartDelay = FMath::RandRange(MovementStartDelayRandomMin, MovementStartDelayRandomMax);
}