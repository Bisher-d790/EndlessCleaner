// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Components/WaypointMovementComponent.h"

UWaypointMovementComponent::UWaypointMovementComponent()
{
	MovementSpeed = 2;
	BeaconSpeed = 25;
	FollowBeaconSpeedMultiplier = 0.5;
	bMovementInLocalSpace = true;
	bUsePingPongWaypointSelection = true;
	bUseBeaconForMovement = true;
	bStopOnLastWaypoint = false;
	StartDelay = 0.0f;
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

	if (StartDelayTimeElapsed < StartDelay)
	{
		StartDelayTimeElapsed += DeltaTime;
		return;
	}

	// Skip if there're no Waypoints
	if (WaypointLocations.Num() < 1) return;

	// Skip if we don't want component updated when not rendered or if updated component can't move
	if (ShouldSkipUpdate(DeltaTime)) return;
	if (!IsValid(UpdatedComponent))	return;

	// Check if the index is out of bounds to get over any changes made by an outsider object
	if (NextWaypointIndex >= WaypointLocations.Num())
		NextWaypointIndex = WaypointLocations.Num() - 1;

	FVector TargetLocation = WaypointLocations[NextWaypointIndex];

	//** Update Beacon Position
	// Movement time duration = distance / speed

	// Check if the movement intitial location is undefined
	if (BeaconPosition.X == nan("ind") || BeaconPosition.Y == nan("ind") || BeaconPosition.Z == nan("ind"))
		BeaconPosition = WaypointLocations[0];

	float LerpDuration = FVector::Dist(BeaconPosition, TargetLocation) / BeaconSpeed;
	BeaconPosition = FMath::Lerp(BeaconPosition, TargetLocation, LerpTimeElapsed / LerpDuration);

	LerpTimeElapsed += DeltaTime;

	//** Update Component Position
	FVector NewLocation;

	// Compute new location
	if (bUseBeaconForMovement)
	{
		// If using Beacon movement, follow the beacon
		FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
		float Distance = MovementSpeed * DeltaTime;
		Distance *= FollowBeaconSpeedMultiplier;
		NewLocation = CurrentLocation + Distance * (BeaconPosition - CurrentLocation);
	}
	else
	{
		// If using Lerping movement, just move with the beacon with out following it
		NewLocation = BeaconPosition;
	}

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
			// Don't change to a next waypoint if this was enabled
			if (bStopOnLastWaypoint) return;

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