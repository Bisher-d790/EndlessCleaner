// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Enemy.h"
#include "Components/SphereComponent.h"
#include "Gameplay/Components/WaypointMovementComponent.h"
#include "Player/EndlessCleanerPlayerController.h"
#include "Player/EndlessCleanerCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemy::AEnemy()
{
	CollisionEnableDelay = 5.f;
	DestructionDelay = 0.5f;

	MovementStartDelayRandomMin = 0.f;
	MovementStartDelayRandomMax = 0.f;

	PerpetualMovementSpeed = FVector::ZeroVector;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle CollisionEnableTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CollisionEnableTimerHandle, this, &AEnemy::OnEnableCollision, CollisionEnableDelay, false);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WaypointMovementComponent->WaypointLocations.Num() <= 0)
	{
		ApplyPerpetualMovement(DeltaTime);
	}
}

void AEnemy::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If had hit a player
	if (IsValid(OtherActor) && OtherActor->IsA<AEndlessCleanerCharacter>())
	{
		// Play Collect SFX
		PlayPickUpCollectedSFX();

		// Disable collision
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FTimerHandle KillTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(KillTimerHandle, this, &AEnemy::KillEnemy, DestructionDelay, false);
	}
}

void AEnemy::KillEnemy()
{
	// Kill enemy logic
	Cast<AEndlessCleanerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->OnKillEnemy(this);
}

void AEnemy::OnEnableCollision()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WaypointMovementComponent->MovementSpeed = Speed;
}

void AEnemy::AddWaypoint(FVector WaypointLocation)
{
	WaypointMovementComponent->WaypointLocations.Add(WaypointLocation);
}

void AEnemy::SetInitialMovementSpeed(float InitialSpeed, float ActualSpeed)
{
	WaypointMovementComponent->MovementSpeed = InitialSpeed;
	Speed = ActualSpeed;
}

void AEnemy::ApplyPerpetualMovement(float DeltaTime)
{
	// Distance = Speed * Time
	FVector Distance = PerpetualMovementSpeed * DeltaTime;

	FVector NewLocation = GetActorLocation() + Distance;

	SetActorLocation(NewLocation);
}