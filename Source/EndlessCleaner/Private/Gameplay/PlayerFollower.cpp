// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlayerFollower.h"
#include "Player/EndlessCleanerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerFollower::APlayerFollower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DistanceThreshold = 0.f;
	LocationDisplacement = FVector::ZeroVector;
	bIsFollowingPlayer = true;
}

// Called every frame
void APlayerFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player == nullptr)
	{
		Player = Cast<AEndlessCleanerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		return;
	}

	// Follow the player
	if (bIsFollowingPlayer && FVector::Dist(GetActorLocation(), Player->GetActorLocation()) >= DistanceThreshold)
	{
		FVector NewLocation = Player->GetActorLocation() + LocationDisplacement;
		SetActorLocation(NewLocation);
	}
}