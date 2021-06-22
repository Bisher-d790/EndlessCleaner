// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlatformsContainer.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
APlatformsContainer::APlatformsContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));
	AddInstanceComponent(RotatingMovementComponent);
}

void APlatformsContainer::RotateLeft(float RotationRate)
{
	RotatingMovementComponent->RotationRate = FRotator(0, 0, RotationRate);
}

void APlatformsContainer::RotateRight(float RotationRate)
{
	RotatingMovementComponent->RotationRate = FRotator(0, 0, -RotationRate);
}

void APlatformsContainer::StopRotation()
{
	RotatingMovementComponent->RotationRate = FRotator::ZeroRotator;
}