// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Pickup.h"
#include "Germ.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSCLEANER_API AGerm : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGerm();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	// The waypoint movement start delay minimum random value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Delay")
		float MovementStartDelayRandomMin;

	// The waypoint movement start delay maximum random value
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Delay")
		float MovementStartDelayRandomMax;

public:
	class UWaypointMovementComponent* WaypointMovementComponent;

};
