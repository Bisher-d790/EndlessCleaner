// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class ENDLESSCLEANER_API AObstacle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObstacle();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle")
		class UBoxComponent* Blocker;

	UFUNCTION()
		void BlockPlayer(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void UnblockPlayer(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
		float ObstacleHitSFXVolume;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
		class USoundBase* ObstacleHitSFX;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class UWaypointMovementComponent* WaypointMovementComponent;

	UFUNCTION(BlueprintCallable, Category = "SFX")
		void PlayObstacleHitSFX();
};