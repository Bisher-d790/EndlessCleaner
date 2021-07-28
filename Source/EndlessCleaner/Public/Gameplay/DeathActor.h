// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/PlayerFollower.h"
#include "DeathActor.generated.h"

UCLASS()
class ENDLESSCLEANER_API ADeathActor : public APlayerFollower
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADeathActor();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UBoxComponent* DeathCollision;

protected:

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
};