// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUIWidget.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSCLEANER_API UInGameUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CoinsCounter;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DistanceCounter;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LivesCounter;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Timer;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ScoreCount;

	UPROPERTY(meta = (BindWidget))
		class UButton* RestartGameButton;

	UPROPERTY(meta = (BindWidget))
		int32 DistanceMinimumFractionalDigits = 0;

	UPROPERTY(meta = (BindWidget))
		int32 DistanceMaximumIntegralDigits = 10;

public:
	void UpdateCoins(int32 Coins);

	void UpdateDistance(float Distance);

	void UpdateLives(int32 Lives);

	void UpdateTime(float Time);

	void SetScore(float Score);

	UFUNCTION(BlueprintCallable, Category = "Buttons Functions")
		void OnClickRestartGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Event")
		void OnStartGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Event")
		void OnGameOver();
};