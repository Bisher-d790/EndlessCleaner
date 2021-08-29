// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Components/WaypointMovementComponent.h"

UWaypointMovementComponent::UWaypointMovementComponent()
{
	MovementSpeed = 2;
	BeaconSpeed = 25;
	FollowBeaconSpeedMultiplier = 0.5;
	bMovementInLocalSpace = true;
}

void UWaypointMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set beacon start position
	BeaconPosition = UpdatedComponent->GetComponentLocation();

	// Set Waypoints in local space
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

	// Skip if we don't want component updated when not rendered or if updated component can't move
	if (ShouldSkipUpdate(DeltaTime)) return;
	if (!IsValid(UpdatedComponent))	return;

	// Compute new location
	FVector TargetLocation = WaypointLocations[NextWaypointIndex];

	//** Update Beacon Position
	// Movement time duration = distance / speed
	float LerpDuration = FVector::Dist(BeaconPosition, TargetLocation) / BeaconSpeed;
	BeaconPosition = FMath::Lerp(BeaconPosition, TargetLocation, LerpTimeElapsed / LerpDuration);

	LerpTimeElapsed += DeltaTime;

	//** Update Component Position
	FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	float Distance = MovementSpeed * DeltaTime;
	Distance *= FollowBeaconSpeedMultiplier;
	FVector NewLocation = CurrentLocation + Distance * (BeaconPosition - CurrentLocation);

	// Set the new location
	UpdatedComponent->SetWorldLocation(NewLocation);

	// If has reached the waypoint (finished lerping), iterate to next waypoint
	if (LerpTimeElapsed >= LerpDuration) NextWaypoint();
}

void UWaypointMovementComponent::NextWaypoint()
{
	if (bIsMovingForward)
	{
		NextWaypointIndex++;

		// When has reached the end
		if (NextWaypointIndex >= WaypointLocations.Num())
		{
			if (bUsePingPongWaypointSelection)
			{
				bIsMovingForward = false;
				NextWaypointIndex = WaypointLocations.Num() > 1 ? WaypointLocations.Num() - 2 : 0;
			}
			else
			{
				NextWaypointIndex = 0;
			}
		}
	}
	else
	{
		NextWaypointIndex--;

		// When has reached the end
		if (NextWaypointIndex < 0)
		{
			bIsMovingForward = true;
			NextWaypointIndex = WaypointLocations.Num() > 1 ? 1 : 0;
		}
	}

	// Reset bIsMovingForward if bUsePingPongWaypointSelection was disabled during gameplay
	if (!bUsePingPongWaypointSelection)	bIsMovingForward = true;

	LerpTimeElapsed = 0;
}

void UWaypointMovementComponent::AddWaypoint(FVector Waypoint)
{
	WaypointLocations.Add(Waypoint);
}
