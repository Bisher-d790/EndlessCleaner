// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUIWidget.h"
#include "Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"
#include "Core/EndlessCleanerGameMode_Level.h"
#include "Kismet/GameplayStatics.h"


void UInGameUIWidget::UpdateCoins(int32 Coins)
{
	if (!IsValid(CoinsCounter)) return;

	CoinsCounter->SetText(FText::FromString(FString::FromInt(Coins)));
}

void UInGameUIWidget::UpdateDistance(float Distance)
{
	if (!IsValid(DistanceCounter)) return;

	float Rounded = roundf(Distance);

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = DistanceMinimumFractionalDigits;
	NumberFormat.MaximumIntegralDigits = DistanceMaximumIntegralDigits;

	FText NewDistance = FText::AsNumber(Rounded, &NumberFormat);

	DistanceCounter->SetText(NewDistance);
}

void UInGameUIWidget::UpdateLives(int32 Lives)
{
	if (!IsValid(LivesCounter)) return;

	LivesCounter->SetText(FText::FromString(FString::FromInt(Lives)));
}

void UInGameUIWidget::UpdateLevel(int32 Level)
{
	if (!IsValid(LevelCounter)) return;

	LevelCounter->SetText(FText::FromString(FString::FromInt(Level)));
}

void UInGameUIWidget::UpdateTime(float Time)
{
	if (!IsValid(Timer)) return;

	float Rounded = roundf(Time);

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = DistanceMinimumFractionalDigits;
	NumberFormat.MaximumIntegralDigits = DistanceMaximumIntegralDigits;

	FText NewTime = FText::AsNumber(Rounded, &NumberFormat);

	Timer->SetText(NewTime);
}


void UInGameUIWidget::SetScore(float Score)
{
	if (!IsValid(ScoreCount)) return;

	float Rounded = roundf(Score);

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = DistanceMinimumFractionalDigits;
	NumberFormat.MaximumIntegralDigits = DistanceMaximumIntegralDigits;

	FText NewScore = FText::AsNumber(Rounded, &NumberFormat);

	ScoreCount->SetText(NewScore);
}

void UInGameUIWidget::OnClickRestartGame()
{
	Cast<AEndlessCleanerGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()))->RestartGame();
}