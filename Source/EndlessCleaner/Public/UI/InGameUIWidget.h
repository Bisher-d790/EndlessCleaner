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
		class UButton* RestartGameButton;

	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HeartsContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Lives")
		class UTexture2D* HeartTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Lives")
		FVector2D HeartSize;

	UPROPERTY(EditDefaultsOnly, Category = "Lives")
		FLinearColor HeartTint;

	void IncreaseHearts();

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* GermsCounter;

public:
	UFUNCTION(BlueprintCallable, Category = "Buttons Functions")
		void OnClickRestartGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI Event")
		void OnStartGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI Event")
		void OnGameOver();

	UFUNCTION(BlueprintCallable, Category = "UI Event")
		void OnLivesChanged(int Lives);

	UFUNCTION(BlueprintCallable, Category = "Lives")
		FORCEINLINE class UHorizontalBox* GetHeartsContainer() { return HeartsContainer; };

	UFUNCTION(BlueprintCallable, Category = "UI Event")
		void OnUpdateGerms(int Germs);
};