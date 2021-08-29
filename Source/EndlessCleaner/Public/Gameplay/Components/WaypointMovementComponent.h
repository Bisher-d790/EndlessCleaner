// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "WaypointMovementComponent.generated.h"

/**
 * Moves the actor across multiple waypoint location points.
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent), HideCategories = (Velocity))
class ENDLESSCLEANER_API UWaypointMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UWaypointMovementComponent();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// The locations of waypoints to move between
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		TArray<FVector> WaypointLocations = TArray<FVector>();

	// Iterate to the next waypoint
	void NextWaypoint();

	int NextWaypointIndex = 0;

	float LerpTimeElapsed = 0;

	bool bIsMovingForward = true;

	// When moving using a beacon system, this the position to follow
	FVector BeaconPosition;

	// Get the beacon position (for debugging purposes)
	UFUNCTION(BlueprintCallable, Category = Debug)
		FVector GetBeaconPosition() { return BeaconPosition; };

public:
	// Add a new waypoint from code
	UFUNCTION(BlueprintCallable, Category = WaypointComponent)
		void AddWaypoint(FVector Waypoint);

	// The movement speed of the object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		float MovementSpeed;

	// The movement speed multiplier of the object when following the beacon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		float FollowBeaconSpeedMultiplier;

	// The movement speed of the beacon that the object follows
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		float BeaconSpeed;

	// Whether waypoint locations are set in local or world space.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		uint32 bMovementInLocalSpace : 1;

	/** Use Pingpong Waypoint Selection or Circular Waypoint Selection.
	* Using PingPong Selection: When reaching the last waypoint, it'll start to move backward through waypoints.
	* Using Circular Selection: When reaching the last waypoint, it'll go back to the first waypoint and start again.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		uint32 bUsePingPongWaypointSelection : 1;

	/** Use Beacon Movement Mechanic or Lerp Movement Mechanic.
	* Using Beacon Mechanic: Lerping a beacon between waypoints, and making the object follow the beacon, provides smoothness of motion.
	* Using Lerp Mechanic: Lerping the object between waypoints, sticks a bit when reaching the waypoint.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		uint32 bUseBeaconForMovement : 1;
};