// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ECPlayerState.h"


AECPlayerState::AECPlayerState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AECPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Reset variables values
	GermsCollected = 0;
	CurrentDistance = 0;
	CurrentTime = 0;
	EnemiesKilled = 0;
}

void AECPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTimerRunning)
	{
		CurrentTime += DeltaTime;
	}
}

void AECPlayerState::IncreaseGermsCollected()
{
	GermsCollected++;

	OnGermCollected.Execute(GermsCollected);
}

void AECPlayerState::ResetGermsCollected()
{
	GermsCollected = 0;

	OnGermCollected.Execute(GermsCollected);
}

void AECPlayerState::DecreaseCurrentLives()
{
	CurrentLives = FMath::Clamp(CurrentLives - 1, 0, CurrentLives);

	OnLivesChange.Execute(CurrentLives);
}

void AECPlayerState::SetCurrentLives(int Lives)
{
	if (Lives < 0) return;

	CurrentLives = Lives;

	OnLivesChange.Execute(CurrentLives);
}

void AECPlayerState::SetIsDead(bool Value)
{
	bIsDead = Value;
}

void AECPlayerState::IncreaseEnemiesKilled()
{
	EnemiesKilled++;

	OnEnemyKilled.Execute(EnemiesKilled);
}

void AECPlayerState::ResetEnemiesKilled()
{
	EnemiesKilled = 0;

	OnEnemyKilled.Execute(EnemiesKilled);
}

void AECPlayerState::AddDistance(float Distance)
{
	if (Distance < 0) return;

	CurrentDistance += Distance;
}

void AECPlayerState::ResetDistance()
{
	CurrentDistance = 0;
}

void AECPlayerState::ResetTimer()
{
	CurrentTime = 0;
}

void AECPlayerState::StopTimer()
{
	bIsTimerRunning = false;
}

void AECPlayerState::StartTimer()
{
	bIsTimerRunning = true;
}
