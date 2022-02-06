// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ECPlayerState.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOneParamChange, int, Param);

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

	int GermsCollected;

	int CurrentLives;

	float CurrentDistance;

	float CurrentTime;

	bool bIsTimerRunning = false;

	bool bIsDead = false;

public:

	FORCEINLINE int GetCoinsCollected() { return GermsCollected; }

	FORCEINLINE int GetCurrentLives() { return CurrentLives; }

	FORCEINLINE float GetCurrentDistance() { return CurrentDistance; }

	FORCEINLINE float GetCurrentTime() { return CurrentTime; }

	FORCEINLINE float GetEnemiesKilled() { return EnemiesKilled; }

	FORCEINLINE bool IsDead() { return bIsDead; }

	void IncreaseGermsCollected();

	void ResetGermsCollected();

	void DecreaseCurrentLives();

	void SetCurrentLives(int Lives);

	void IncreaseEnemiesKilled();

	void ResetEnemiesKilled();

	void AddDistance(float Distance);

	void SetIsDead(bool Value);

	void ResetDistance();

	void ResetTimer();

	void StopTimer();

	void StartTimer();

	UPROPERTY()
		FOneParamChange OnLivesChange;

	UPROPERTY()
		FOneParamChange OnGermCollected;
};
