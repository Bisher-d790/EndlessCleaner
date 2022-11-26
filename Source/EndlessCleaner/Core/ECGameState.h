// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ECGameState.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSCLEANER_API AECGameState : public AGameState
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;

	int CurrentLevel;

public:
	UFUNCTION(BlueprintPure, Category = "Level")
		FORCEINLINE int GetCurrentLevel() { return CurrentLevel; }

	UFUNCTION(BlueprintCallable, Category = "Level")
		void IncreaseCurrentLevel();

	UFUNCTION(BlueprintCallable, Category = "Level")
		void ResetLevel();
};
