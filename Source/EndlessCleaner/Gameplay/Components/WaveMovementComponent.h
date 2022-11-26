// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "WaveMovementComponent.generated.h"

/**
 * Performs ping-pong rotation of a component at a specific rotation rate between two angles.
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent), HideCategories = (Velocity))
class ENDLESSCLEANER_API UWaveMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UWaveMovementComponent();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// The rotation of the wave start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveComponent)
		TArray<FRotator> WaveRotations = TArray<FRotator>();

	// The duration of the rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveComponent)
		float RotationDuration;

	// The duration of the rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveComponent)
		float RotationEdgeTolerance;

	// Start from a random index
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WaveComponent)
		bool bRandomStartIndex = false;

	void NextWaveRotation();

	int NextIndex = 0;

	float LerpTimeElapsed = 0;
};
