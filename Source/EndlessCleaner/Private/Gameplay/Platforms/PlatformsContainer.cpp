// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Platforms/PlatformsContainer.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
APlatformsContainer::APlatformsContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RotationSpeed = 20.f;

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));
	AddInstanceComponent(RotatingMovementComponent);
}

void APlatformsContainer::RotateLeft()
{
	if (!bIsRotating)
		RotationBeforeLastMovement = GetActorRotation().GetDenormalized();

	RotatingMovementComponent->RotationRate = FRotator(0, 0, RotationSpeed);

	bIsRotating = true;
}

void APlatformsContainer::RotateRight()
{
	if (!bIsRotating)
		RotationBeforeLastMovement = GetActorRotation().GetDenormalized();

	RotatingMovementComponent->RotationRate = FRotator(0, 0, -RotationSpeed);

	bIsRotating = true;
}

void APlatformsContainer::StopRotation()
{
	RotatingMovementComponent->RotationRate = FRotator::ZeroRotator;

	bIsRotating = false;
}

float APlatformsContainer::GetCurrentMovementRotationInDegrees()
{
	float Rotation = 0.f;

	if (bIsRotating)
	{
		Rotation = FMath::Abs(RotationBeforeLastMovement.Roll - GetActorRotation().GetDenormalized().Roll);
		//UE_LOG(LogTemp, Warning, TEXT("Rotaiton: %.2f"), Rotation);
	}

	return Rotation;
}