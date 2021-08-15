// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Components/WaveMovementComponent.h"
#include "Core/EndlessCleanerGameMode_Level.h"


UWaveMovementComponent::UWaveMovementComponent()
{
	WaveStartRotation.Yaw = -60.f;
	WaveEndRotation.Yaw = 60.f;
	RotationRate = 100;
	RotationEdgeTolerance = 10.f;
	bRotationInLocalSpace = true;
}

void UWaveMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Reset the rotation to the start of the wave
	MoveUpdatedComponent(FVector::ZeroVector, FQuat(WaveStartRotation), false);

	bIsRotatingForward = true;
}

void UWaveMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Skip if the two rotation edges are equal
	if (WaveEndRotation.Equals(WaveStartRotation)) return;

	// skip if we don't want component updated when not rendered or if updated component can't move
	if (ShouldSkipUpdate(DeltaTime)) return;
	if (!IsValid(UpdatedComponent))	return;

	// Calculate the rotation direction
	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation();
	FRotator TargetRotation = bIsRotatingForward ? WaveEndRotation : WaveStartRotation;

	FRotator RotationDirection = (TargetRotation - CurrentRotation);
	RotationDirection.Normalize();

	// Set the rotation rate
	if (FMath::IsNearlyZero(RotationDirection.Yaw))
		RotationDirection.Yaw = RotationDirection.Yaw > 0.0f ? RotationRate : -RotationRate;
	if (FMath::IsNearlyZero(RotationDirection.Pitch))
		RotationDirection.Pitch = RotationDirection.Pitch > 0.0f ? RotationRate : -RotationRate;
	if (FMath::IsNearlyZero(RotationDirection.Roll))
		RotationDirection.Roll = RotationDirection.Roll > 0.0f ? RotationRate : -RotationRate;

	// Compute new rotation
	const FQuat OldRotation = UpdatedComponent->GetComponentQuat();
	const FQuat DeltaRotation = (RotationDirection * DeltaTime).Quaternion();
	const FQuat NewRotation = bRotationInLocalSpace ? (OldRotation * DeltaRotation) : (DeltaRotation * OldRotation);

	// Compute new location
	FVector DeltaLocation = FVector::ZeroVector;
	if (!PivotTranslation.IsZero())
	{
		const FVector OldPivot = OldRotation.RotateVector(PivotTranslation);
		const FVector NewPivot = NewRotation.RotateVector(PivotTranslation);
		DeltaLocation = (OldPivot - NewPivot); // ConstrainDirectionToPlane() not necessary because it's done by MoveUpdatedComponent() below.
	}

	const bool bEnableCollision = false;
	MoveUpdatedComponent(DeltaLocation, NewRotation, bEnableCollision);

	// If has reached one edge of the wave, flip the rotation direction
	if (CurrentRotation.Equals(TargetRotation, RotationEdgeTolerance))
		bIsRotatingForward = !bIsRotatingForward;
}