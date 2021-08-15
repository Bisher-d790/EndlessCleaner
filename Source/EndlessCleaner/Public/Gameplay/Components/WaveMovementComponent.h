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
		FRotator WaveStartRotation;

	// The rotation of the wave end
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveComponent)
		FRotator WaveEndRotation;

	// The speed of the rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveComponent)
		float RotationRate;

	// The tolerance of detecting a Start or Edge Rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveComponent)
		float RotationEdgeTolerance;

	/**
	 * Translation of pivot point around which we rotate, relative to current rotation.
	 * For instance, with PivotTranslation set to (X=+100, Y=0, Z=0), rotation will occur
	 * around the point +100 units along the local X axis from the center of the object,
	 * rather than around the object's origin (the default).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveComponent)
		FVector PivotTranslation;

	/** Whether rotation is applied in local or world space. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveComponent)
		uint32 bRotationInLocalSpace : 1;

	bool bIsRotatingForward;
};