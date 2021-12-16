// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ECPlayerState.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSCLEANER_API AECPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AECPlayerState();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	int EnemiesKilled;

	int CoinsCollected;

	int CurrentLives;

	float CurrentDistance;

	float CurrentTime;

	bool bIsTimerRunning = false;

public:

	FORCEINLINE int GetCoinsCollected() { return CoinsCollected; }

	FORCEINLINE int GetCurrentLives() { return CurrentLives; }

	FORCEINLINE float GetCurrentDistance() { return CurrentDistance; }

	FORCEINLINE float GetCurrentTime() { return CurrentTime; }

	FORCEINLINE float GetEnemiesKilled() { return EnemiesKilled; }

	void IncreaseCoinsCollected();

	void ResetCoinsCollected();

	void DecreaseCurrentLives();

	void SetCurrentLives(int Lives);

	void IncreaseEnemiesKilled();

	void ResetEnemiesKilled();

	void AddDistance(float Distance);

	void ResetDistance();

	void ResetTimer();

	void StopTimer();

	void StartTimer();
};
