// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "PerpetualMovementComponent.generated.h"


UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class ENDLESSCLEANER_API UPerpetualMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPerpetualMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class AECCharacter* Player;

	FVector CachedVelocity;

	// The movement speed of the object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bIsMovementEnabled;

	// Modify speed according to the ditstance from the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bTrackDistanceFromPlayer;

	// Distance from the player to retract the velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float DistanceToRetractVelocity;

	// Velocity when the enemy is retracting to keep the player within vicinity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		FVector RetractionVelocity;

	// Time buffer to end velocity retraction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float VelocityRetractionStartEndDelay;

	bool bIsRetractingVelocity = false;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RetractVelocity();

	void EndVelocityRetraction();

	FORCEINLINE void SetRetractionVelocity(FVector Value) { this->RetractionVelocity = Value; }

	FORCEINLINE void SetVelocity(FVector Value) { Velocity = Value; }
};
