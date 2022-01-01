// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Components/PerpetualMovementComponent.h"

// Sets default values for this component's properties
UPerpetualMovementComponent::UPerpetualMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsMovementEnabled = true;
}


// Called when the game starts
void UPerpetualMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UPerpetualMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// Skip if we don't want component updated when not rendered or if updated component can't move
	if (ShouldSkipUpdate(DeltaTime)) return;
	if (!IsValid(UpdatedComponent))	return;
	if (!bIsMovementEnabled) return;

	FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	FVector TargetLocation = CurrentLocation + (Velocity * DeltaTime);

	UpdatedComponent->SetWorldLocation(TargetLocation);
}

