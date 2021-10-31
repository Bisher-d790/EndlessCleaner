// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Obstacle.h"
#include "Gameplay/Components/WaypointMovementComponent.h"
#include "Player/EndlessCleanerCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AObstacle::AObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentHit.AddDynamic(this, &AObstacle::OnHit);

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
	if (IsValid(OtherActor) && OtherActor->IsA<AEndlessCleanerCharacter>())
	{
		// Play hit SFX
		PlayObstacleHitSFX();
	}
}