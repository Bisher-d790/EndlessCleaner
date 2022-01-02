// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Components/PerpetualMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ECCharacter.h"
#include "Utils/Debug.h"


// Sets default values for this component's properties
UPerpetualMovementComponent::UPerpetualMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsMovementEnabled = true;
	bTrackDistanceFromPlayer = true;
	DistanceToRetractVelocity = 10000.f;
	RetractionVelocity = Velocity / 4;
	VelocityRetractionStartEndDelay = 0.1f;
}

// Called when the game starts
void UPerpetualMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache the set velocity
	CachedVelocity = Velocity;

	// Get the player reference
	Player = Cast<AECCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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

	if (bTrackDistanceFromPlayer && IsValid(Player))
	{
		FVector PlayerLocation = Player->GetActorLocation();
		if (FMath::Abs(FVector::Distance(PlayerLocation, TargetLocation)) >= DistanceToRetractVelocity
			&& PlayerLocation.X < TargetLocation.X)
		{
			FTimerHandle RetractionTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(RetractionTimerHandle, this, &UPerpetualMovementComponent::RetractVelocity, VelocityRetractionStartEndDelay, false);
		}
		else if (bIsRetractingVelocity)
		{
			FTimerHandle RetractionTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(RetractionTimerHandle, this, &UPerpetualMovementComponent::EndVelocityRetraction, VelocityRetractionStartEndDelay, false);
		}
	}
}

void UPerpetualMovementComponent::RetractVelocity()
{
	if (bIsRetractingVelocity) return;

	CachedVelocity = Velocity;
	Velocity = RetractionVelocity;
	bIsRetractingVelocity = true;

	PrintDebugLog("Start Retraction");
}

void UPerpetualMovementComponent::EndVelocityRetraction()
{
	if (!bIsRetractingVelocity) return;

	// Reset to the cached Velocity 
	Velocity = CachedVelocity;
	bIsRetractingVelocity = false;

	PrintDebugLog("End Retraction");
}