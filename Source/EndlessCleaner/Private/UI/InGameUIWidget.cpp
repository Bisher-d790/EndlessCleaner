// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUIWidget.h"
#include "Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"
#include "Core/EndlessCleanerGameModeBase.h"
#include "Kismet/GameplayStatics.h"


void UInGameUIWidget::UpdateCoins(int32 Coins)
{
	if (!CoinsCounter) return;

	CoinsCounter->SetText(FText::FromString(FString::FromInt(Coins)));
}

void UInGameUIWidget::UpdateDistance(float Distance)
{
	if (!DistanceCounter) return;

	float Rounded = roundf(Distance);

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = DistanceMinimumFractionalDigits;
	NumberFormat.MaximumIntegralDigits = DistanceMaximumIntegralDigits;

	FText NewDistance = FText::AsNumber(Rounded, &NumberFormat);

	DistanceCounter->SetText(NewDistance);
}

void UInGameUIWidget::UpdateLives(int32 Lives)
{
	if (!LivesCounter) return;

	LivesCounter->SetText(FText::FromString(FString::FromInt(Lives)));
}

void UInGameUIWidget::UpdateTime(float Time)
{
	if (!Timer) return;

	float Rounded = roundf(Time);

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = DistanceMinimumFractionalDigits;
	NumberFormat.MaximumIntegralDigits = DistanceMaximumIntegralDigits;

	FText NewTime = FText::AsNumber(Rounded, &NumberFormat);

	Timer->SetText(NewTime);
}


void UInGameUIWidget::SetScore(float Score)
{
	if (!ScoreCount) return;

	float Rounded = roundf(Score);

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = DistanceMinimumFractionalDigits;
	NumberFormat.MaximumIntegralDigits = DistanceMaximumIntegralDigits;

	FText NewScore = FText::AsNumber(Rounded, &NumberFormat);

	ScoreCount->SetText(NewScore);
}

void UInGameUIWidget::OnClickRestartGame()
{
	Cast<AEndlessCleanerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->RestartGame();
}