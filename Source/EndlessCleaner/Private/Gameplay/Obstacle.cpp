// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Obstacle.h"
#include "Gameplay/Components/WaypointMovementComponent.h"

// Sets default values
AObstacle::AObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	WaypointMovementComponent = CreateDefaultSubobject<UWaypointMovementComponent>(TEXT("Waypoint Movement"));
	AddInstanceComponent(WaypointMovementComponent);
}
