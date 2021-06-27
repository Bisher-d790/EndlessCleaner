// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LaneComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENDLESSCLEANER_API ULaneComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULaneComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Lane")
		float LaneWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		float PickupProbability;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		FVector PickupStartPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		FVector DistanceBetweenPickups;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		int PickupsNumberPerSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		TSubclassOf<class APickup> PickupClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
		float ObstacleProbability;

	UPROPERTY(EditDefaultsOnly, Category = "Obstacle")
		FVector ObstaclePosition;

	UPROPERTY(EditDefaultsOnly, Category = "Obstacle")
		TSubclassOf<class AObstacle> ObstacleClass = nullptr;
};
