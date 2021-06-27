// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformsContainer.generated.h"

UCLASS()
class ENDLESSCLEANER_API APlatformsContainer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlatformsContainer();

public:
	void RotateLeft(float RotationRate);

	void RotateRight(float RotationRate);

	void StopRotation();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class URotatingMovementComponent* RotatingMovementComponent;
};
