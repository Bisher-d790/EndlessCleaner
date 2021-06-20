// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DeathActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/EndlessCleanerGameMode_Level.h"
#include "Player/EndlessCleanerCharacter.h"

// Sets default values
ADeathActor::ADeathActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DeathCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = DeathCollision;

	DeathCollision->OnComponentBeginOverlap.AddDynamic(this, &ADeathActor::OnBeginOverlap);

	PlayerRef = Cast<AEndlessCleanerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	DistanceThresholdToReposition = 500.f;
	bMoveAlongTheXAxis = false;
	bMoveAlongTheYAxis = false;
	bMoveAlongTheZAxis = false;
	bMoveWithPlayer = false;
}

void ADeathActor::Tick(float DeltaTime)
{
	if (!PlayerRef)
		PlayerRef = Cast<AEndlessCleanerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Move with character
	if (bMoveWithPlayer &&
		FVector::Dist(PlayerRef->GetActorLocation(), GetActorLocation()) >= DistanceThresholdToReposition)
	{
		FVector NewLocation = GetActorLocation();

		if (bMoveAlongTheXAxis) NewLocation.X = PlayerRef->GetActorLocation().X;
		if (bMoveAlongTheYAxis) NewLocation.Y = PlayerRef->GetActorLocation().Y;
		if (bMoveAlongTheZAxis) NewLocation.Z = PlayerRef->GetActorLocation().Z;

		SetActorLocation(NewLocation);
	}
}

void ADeathActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Cast<AEndlessCleanerGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()))->OnTriggerDeathActor();
}