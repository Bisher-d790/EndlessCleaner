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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	bool bIsRotating = false;

	FRotator RotationBeforeLastMovement = FRotator();

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float RotationSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		FRotator InitialRotation;

public:
	void RotateLeft();

	void RotateRight();

	void StopRotation();

	void ResetRotation();

	float GetCurrentMovementRotationInDegrees();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class URotatingMovementComponent* RotatingMovementComponent;

	FORCEINLINE bool GetIsRotating() { return bIsRotating; };
};
