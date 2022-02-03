// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUIWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Core/ECGameMode_Level.h"
#include "Kismet/GameplayStatics.h"


void UInGameUIWidget::OnClickRestartGame()
{
	Cast<AECGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()))->RestartGame();
}

void UInGameUIWidget::OnLivesChanged(int Lives)
{
	if (!IsValid(HeartsContainer))
		return;

	HeartsContainer->ClearChildren();

	for (int i = 0; i < Lives; i++)
	{
		IncreaseHearts();
	}
}

void UInGameUIWidget::IncreaseHearts()
{
	UImage* Heart = NewObject<UImage>(UImage::StaticClass());

	if (!IsValid(Heart) || !IsValid(HeartsContainer) || !IsValid(HeartTexture))
		return;

	Heart->SetVisibility(ESlateVisibility::Visible);
	Heart->SetBrushFromTexture(HeartTexture);
	Heart->SetBrushSize(HeartSize);
	Heart->SetColorAndOpacity(HeartTint);

	HeartsContainer->AddChild(Heart);
}