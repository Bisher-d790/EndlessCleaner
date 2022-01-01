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

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// The movement speed of the object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Component")
		bool bIsMovementEnabled;
};
