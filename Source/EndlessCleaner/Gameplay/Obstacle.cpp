// Fill out your copyright notice in the Description page of Project Settings.

#include "Obstacle.h"
#include "Components/WaypointMovementComponent.h"
#include "EndlessCleaner/Player/ECCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"


// Sets default values
AObstacle::AObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentHit.AddDynamic(this, &AObstacle::OnHit);

	Blocker = CreateDefaultSubobject<UBoxComponent>(TEXT("Blocker"));
	Blocker->SetupAttachment(RootComponent);
	Blocker->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::BlockPlayer);
	Blocker->OnComponentEndOverlap.AddDynamic(this, &AObstacle::UnblockPlayer);

	WaypointMovementComponent = CreateDefaultSubobject<UWaypointMovementComponent>(TEXT("Waypoint Movement"));
	AddInstanceComponent(WaypointMovementComponent);

	ObstacleHitSFXVolume = 1.0f;
}

void AObstacle::PlayObstacleHitSFX()
{
	if (IsValid(ObstacleHitSFX))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ObstacleHitSFX, GetActorLocation(), ObstacleHitSFXVolume);
}

void AObstacle::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// If had hit a player
	if (IsValid(OtherActor) && OtherActor->IsA<AECCharacter>())
	{
		// Play hit SFX
		PlayObstacleHitSFX();
	}
}

void AObstacle::BlockPlayer(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AECCharacter* Player = Cast<AECCharacter>(OtherActor);
	if (IsValid(Player))
	{
		Player->BlockForwardMovement(true);
	}
}

void AObstacle::UnblockPlayer(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AECCharacter* Player = Cast<AECCharacter>(OtherActor);
	if (IsValid(Player))
	{
		Player->BlockForwardMovement(false);
	}
}