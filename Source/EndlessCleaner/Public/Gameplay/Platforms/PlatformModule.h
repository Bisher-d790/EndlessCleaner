// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Definitions.h"
#include "PlatformModule.generated.h"

// A struct to store the lane values
USTRUCT(BlueprintType)
struct FLaneOptions
{
	GENERATED_USTRUCT_BODY()

public:

	FLaneOptions()
	{
		PickupStartPosition = FVector(300.f, 0.f, 0.f);
		DistanceBetweenPickups = FVector(300.f, 0.f, 0.f);
		PickupsNumberPerSpawn = 3;
		LaneWidth = 100.0f;
		PickupProbability = 50.f;
		ObstacleProbability = 0.f;
		ObstaclePosition = FVector(0.f, 0.f, 150.f);
	}

	UPROPERTY(EditDefaultsOnly, Category = "Platform")
		FVector LanePosition;

	UPROPERTY(EditDefaultsOnly, Category = "Platform")
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

UCLASS()
class ENDLESSCLEANER_API APlatformModule : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlatformModule();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
		class USceneComponent* StartModulePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
		class USceneComponent* EndModulePoint;

	UPROPERTY(EditDefaultsOnly, Category = "Platform")
		EPlatformType PlatformType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Lanes")
		TArray<FLaneOptions> Lanes;

	APlatformModule* PreviousPlatform = nullptr;

	APlatformModule* NextPlatform = nullptr;

	TArray<class APickup*> SpawnedPickups;

	TArray<class AObstacle*> SpawnedObstacles;

	float PlatformLength;

public:
	FORCEINLINE float GetPlatformLength() { return PlatformLength; };

	FORCEINLINE EPlatformType GetPlatformType() { return PlatformType; };

	FORCEINLINE APlatformModule* GetNextPlatform() { return NextPlatform; };

	FORCEINLINE void SetNextPlatform(APlatformModule* Value) { NextPlatform = Value; };

	FORCEINLINE APlatformModule* GetPreviousPlatform() { return PreviousPlatform; };

	FORCEINLINE void SetPreviousPlatform(APlatformModule* Value) { PreviousPlatform = Value; };

	FORCEINLINE TArray<FLaneOptions> GetLanesArray() { return Lanes; };

	void DestroyPlatform();

	void SpawnPickups();

	void SpawnObstacles();
};