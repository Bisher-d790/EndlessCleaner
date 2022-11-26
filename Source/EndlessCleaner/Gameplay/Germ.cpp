// Fill out your copyright notice in the Description page of Project Settings.

#include "Germ.h"
#include "Components/WaypointMovementComponent.h"


// Sets default values
AGerm::AGerm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MovementComponent = CreateDefaultSubobject<UWaypointMovementComponent>(TEXT("Waypoint Movement"));
	AddInstanceComponent(MovementComponent);

	WaypointMovementComponent = Cast<UWaypointMovementComponent>(MovementComponent);

	MovementStartDelayRandomMin = 0.f;
	MovementStartDelayRandomMax = 1.f;

	PickUpCollectedSFXVolume = 1.0f;
}

void AGerm::BeginPlay()
{
	Super::BeginPlay();

	WaypointMovementComponent->StartDelay = FMath::RandRange(MovementStartDelayRandomMin, MovementStartDelayRandomMax);
}