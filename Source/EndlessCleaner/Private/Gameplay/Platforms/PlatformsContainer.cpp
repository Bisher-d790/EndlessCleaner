// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Platforms/PlatformsContainer.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Gameplay/Platforms/PlatformModule.h" 
#include "Utils/Debug.h"


// Sets default values
APlatformsContainer::APlatformsContainer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialRotation = FRotator::ZeroRotator;

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));
	AddInstanceComponent(RotatingMovementComponent);
}

// Called when the game starts or when spawned
void APlatformsContainer::BeginPlay()
{
	Super::BeginPlay();

	ResetRotation();
}

void APlatformsContainer::RotateLeft(float RotationRate)
{
	if (!bIsRotating)
		RotationBeforeLastMovement = GetActorRotation().GetDenormalized();

	RotatingMovementComponent->RotationRate = FRotator(0, 0, RotationRate);

	bIsRotating = true;

	for (auto& Platform : PlatformModules)
		if (IsValid(Platform)) Platform->OnRotatePlatform(true);
}

void APlatformsContainer::RotateRight(float RotationRate)
{
	if (!bIsRotating)
		RotationBeforeLastMovement = GetActorRotation().GetDenormalized();

	RotatingMovementComponent->RotationRate = FRotator(0, 0, -RotationRate);

	bIsRotating = true;

	for (auto& Platform : PlatformModules)
		if (IsValid(Platform)) Platform->OnRotatePlatform(false);
}

void APlatformsContainer::StopRotation()
{
	RotatingMovementComponent->RotationRate = FRotator::ZeroRotator;

	bIsRotating = false;
}

void APlatformsContainer::ResetRotation()
{
	StopRotation();

	SetActorRotation(InitialRotation);
}

float APlatformsContainer::GetCurrentMovementRotationInDegrees()
{
	float Rotation = 0.f;

	if (bIsRotating)
	{
		Rotation = FMath::Abs(RotationBeforeLastMovement.Roll - GetActorRotation().GetDenormalized().Roll);
		PrintDebugLog(FString::Printf(TEXT("Rotaiton: %.2f"), Rotation));
	}

	return Rotation;
}