// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveMovementComponent.h"
#include "EndlessCleaner/Core/ECGameMode_Level.h"


UWaveMovementComponent::UWaveMovementComponent()
{
	RotationDuration = 5;
	RotationEdgeTolerance = 1.f;
}

void UWaveMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Reset the rotation to the start of the wave
	if (WaveRotations.Num() > 0)
	{
		FRotator WaveStartRotation = WaveRotations[0];
		UpdatedComponent->SetRelativeRotation(WaveStartRotation);

		// Randomly select the first index
		if (bRandomStartIndex)	NextIndex = FMath::RandRange(0, WaveRotations.Num() - 1);
	}
}

void UWaveMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Skip if there're no wave rotations, or there's only one rotation
	if (WaveRotations.Num() <= 1) return;

	// skip if we don't want component updated when not rendered or if updated component can't move
	if (ShouldSkipUpdate(DeltaTime)) return;
	if (!IsValid(UpdatedComponent))	return;

	// Compute new Rotation
	FRotator CurrentRotation = UpdatedComponent->GetRelativeRotation();
	FRotator TargetRotation = WaveRotations[NextIndex];

	FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, LerpTimeElapsed / RotationDuration);

	LerpTimeElapsed += DeltaTime;

	UpdatedComponent->SetRelativeRotation(NewRotation);

	if (LerpTimeElapsed >= RotationDuration
		|| CurrentRotation.Equals(TargetRotation, RotationEdgeTolerance))
		NextWaveRotation();
}

void UWaveMovementComponent::NextWaveRotation()
{
	NextIndex++;
	if (NextIndex >= WaveRotations.Num()) NextIndex = 0;

	LerpTimeElapsed = 0;
}