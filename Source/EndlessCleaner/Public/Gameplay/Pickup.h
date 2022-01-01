// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class ENDLESSCLEANER_API APickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
		class USphereComponent* Collision;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
		float PickUpCollectedSFXVolume;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
		class USoundBase* PickUpCollectedSFX;

	UFUNCTION()
		virtual void OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class UMovementComponent* MovementComponent;

public:
	UFUNCTION(BlueprintCallable, Category = "SFX")
		void PlayPickUpCollectedSFX();
};
