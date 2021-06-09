// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Definitions.h"
#include "PlatformModule.generated.h"

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

	UPROPERTY(EditDefaultsOnly, Category = "Platform Settings")
		EPlatformType PlatformType;

	UPROPERTY(EditDefaultsOnly, Category = "Platform Settings")
		float PlatformLength;

	UPROPERTY(EditDefaultsOnly, Category = "Platform Settings")
		float LaneWidth;

	APlatformModule* PreviousPlatform = nullptr;

	APlatformModule* NextPlatform = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Settings")
		float DistanceBetweenLanes;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Settings")
		float PickupStartLocationX;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Settings")
		float DistanceBetweenPickupsX;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Settings")
		int PickupsNumberPerSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Settings")
		TSubclassOf<class APickup> PickupClass = nullptr;

	TArray<class APickup*> SpawnedPickups;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup Settings")
		USceneComponent* Lane0; // Left Lane

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup Settings")
		USceneComponent* Lane1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup Settings")
		USceneComponent* Lane2; // Right Lane

public:
	FORCEINLINE float GetPlatformLength() { return PlatformLength; };

	FORCEINLINE EPlatformType GetPlatformType() { return PlatformType; };

	FORCEINLINE APlatformModule* GetNextPlatform() { return NextPlatform; };

	FORCEINLINE void SetNextPlatform(APlatformModule* Value) { NextPlatform = Value; };

	FORCEINLINE APlatformModule* GetPreviousPlatform() { return PreviousPlatform; };

	FORCEINLINE void SetPreviousPlatform(APlatformModule* Value) { PreviousPlatform = Value; };

	FORCEINLINE float GetLaneWidth() { return LaneWidth; };

	TArray<FVector> GetLanesArray();

	void DestroyPlatform();

	void SpawnPickups(int PlatformCount);
};
