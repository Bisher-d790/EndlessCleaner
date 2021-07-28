// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerFollower.generated.h"

UCLASS()
class ENDLESSCLEANER_API APlayerFollower : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerFollower();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bIsFollowingPlayer;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float DistanceThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		FVector LocationDisplacement;

	class AEndlessCleanerCharacter* Player;
};
