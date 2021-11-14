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
		FRotator InitialRotation;

	TArray<class APlatformModule*> PlatformModules = TArray<class APlatformModule*>();

public:
	void RotateLeft(float RotationRate);

	void RotateRight(float RotationRate);

	void StopRotation();

	void ResetRotation();

	float GetCurrentMovementRotationInDegrees();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class URotatingMovementComponent* RotatingMovementComponent;

	FORCEINLINE bool GetIsRotating() { return bIsRotating; };

	FORCEINLINE void AddPlatformModule(APlatformModule* PlatformRef) { PlatformModules.Add(PlatformRef); };

	FORCEINLINE void RemovePlatformModule(APlatformModule* PlatformRef) { PlatformModules.Remove(PlatformRef); };
};