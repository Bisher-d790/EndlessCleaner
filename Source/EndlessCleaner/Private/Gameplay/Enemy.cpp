// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Enemy.h"
#include "Components/SphereComponent.h"
#include "Gameplay/Components/PerpetualMovementComponent.h"
#include "Player/ECPlayerController.h"
#include "Player/ECCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemy::AEnemy()
{
	CollisionEnableDelay = 5.f;
	DestructionDelay = 0.5f;

	MovementComponent = CreateDefaultSubobject<UPerpetualMovementComponent>(TEXT("Perpetual Movement"));
	AddInstanceComponent(MovementComponent);

	PerpetualMovementComponent = Cast<UPerpetualMovementComponent>(MovementComponent);
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
}

void AEnemy::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If had hit a player
	if (IsValid(OtherActor) && OtherActor->IsA<AECCharacter>())
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
	Cast<AECPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->OnKillEnemy(this);
}

void AEnemy::OnEnableCollision()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}