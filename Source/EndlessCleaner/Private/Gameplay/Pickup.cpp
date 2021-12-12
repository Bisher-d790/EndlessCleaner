// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Pickup.h"
#include "Components/SphereComponent.h"
#include "Gameplay/Components/WaypointMovementComponent.h"
#include "Player/ECCharacter.h"
#include "Player/ECPlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapped);

	WaypointMovementComponent = CreateDefaultSubobject<UWaypointMovementComponent>(TEXT("Waypoint Movement"));
	AddInstanceComponent(WaypointMovementComponent);

	MovementStartDelayRandomMin = 0.f;
	MovementStartDelayRandomMax = 1.f;

	PickUpCollectedSFXVolume = 1.0f;
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	WaypointMovementComponent->StartDelay = FMath::RandRange(MovementStartDelayRandomMin, MovementStartDelayRandomMax);
}

void APickup::PlayPickUpCollectedSFX()
{
	if (IsValid(PickUpCollectedSFX))
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpCollectedSFX, GetActorLocation(), PickUpCollectedSFXVolume);
}

void APickup::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If had hit a player
	if (IsValid(OtherActor) && OtherActor->IsA<AECCharacter>())
	{
		// Collect coin logic
		Cast<AECPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->OnCollectCoin();

		// Disable collision
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Play Collect SFX
		PlayPickUpCollectedSFX();
	}
}