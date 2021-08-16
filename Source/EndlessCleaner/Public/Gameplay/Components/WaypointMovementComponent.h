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
	// The rotation of the wave start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		TArray<FVector> WaypointLocations = TArray<FVector>();

	// The speed of the rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		float MovementSpeed;

	// Whether movement is applied in local or world space.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaypointComponent)
		uint32 bMovementInLocalSpace : 1;

	void NextWaypoint();

	int NextIndex = 0;

	float LerpTimeElapsed = 0;
};