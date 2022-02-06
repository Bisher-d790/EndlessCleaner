// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameUIWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
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

void UInGameUIWidget::OnUpdateGerms(int Germs)
{
	if (Germs < 0 || !IsValid(GermsCounter))
		return;

	GermsCounter->SetText(FText::FromString(FString::FromInt(Germs)));
}

void UInGameUIWidget::OnUpdateViruses(int Viruses)
{
	if (Viruses < 0 || !IsValid(VirusCounter))
		return;

	VirusCounter->SetText(FText::FromString(FString::FromInt(Viruses)));
}