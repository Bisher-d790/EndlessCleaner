// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Pickup.h"
#include "Enemy.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSCLEANER_API AEnemy : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	virtual void OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// The amount of time the collision will be deactivated on start
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
		float CollisionEnableDelay;

private:
	void OnEnableCollision();

	float Speed;

public:
	void AddWaypoint(FVector WaypointLocation);

	void SetInitialMovementSpeed(float InitialSpeed, float ActualSpeed);
};
