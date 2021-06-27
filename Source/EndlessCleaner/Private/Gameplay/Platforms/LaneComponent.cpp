// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Platforms/LaneComponent.h"

// Sets default values for this component's properties
ULaneComponent::ULaneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PickupStartPosition = FVector(300.f, 0.f, 0.f);
	DistanceBetweenPickups = FVector(300.f, 0.f, 0.f);
	PickupsNumberPerSpawn = 3;
	LaneWidth = 100.0f;
	PickupProbability = 50.f;
	ObstacleProbability = 0.f;
	ObstaclePosition = FVector(0.f, 0.f, 150.f);
}