// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathActor.generated.h"

UCLASS()
class ENDLESSCLEANER_API ADeathActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADeathActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UBoxComponent* DeathCollision;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		bool bMoveWithPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		bool bMoveAlongTheXAxis;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		bool bMoveAlongTheYAxis;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		bool bMoveAlongTheZAxis;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float DistanceThresholdToReposition;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

private:
	class AEndlessCleanerCharacter* PlayerRef;
};