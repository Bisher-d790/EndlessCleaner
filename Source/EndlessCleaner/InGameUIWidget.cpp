// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUIWidget.h"
#include "Components/TextBlock.h"

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