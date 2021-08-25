// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Components/WaypointMovementComponent.h"

UWaypointMovementComponent::UWaypointMovementComponent()
{
	MovementSpeed = 10;
	bMovementInLocalSpace = true;
}

void UWaypointMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bMovementInLocalSpace)
	{
		FVector ActorLocation = UpdatedComponent->GetComponentLocation();
		for (int i = 0; i < WaypointLocations.Num(); i++)
		{
			WaypointLocations[i] += ActorLocation;
		}
	}
}

void UWaypointMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Skip if there're no Waypoints, or there's only one waypoint
	if (WaypointLocations.Num() <= 1) return;

	// skip if we don't want component updated when not rendered or if updated component can't move
	if (ShouldSkipUpdate(DeltaTime)) return;
	if (!IsValid(UpdatedComponent))	return;

	// Compute new location
	FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	FVector TargetLocation = WaypointLocations[NextIndex];

	// Movement time duration = distance / speed
	float LerpDuration = FVector::Dist(CurrentLocation, TargetLocation) / MovementSpeed;
	FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, LerpTimeElapsed / LerpDuration);

	LerpTimeElapsed += DeltaTime;

	UpdatedComponent->SetWorldLocation(NewLocation);

	if (LerpTimeElapsed >= LerpDuration) NextWaypoint();
}

void UWaypointMovementComponent::NextWaypoint()
{
	NextIndex++;
	if (NextIndex >= WaypointLocations.Num()) NextIndex = 0;

	LerpTimeElapsed = 0;
}