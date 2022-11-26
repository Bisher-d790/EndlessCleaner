// Fill out your copyright notice in the Description page of Project Settings.

#include "ECGameState.h"


void AECGameState::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevel = 1;
}

void AECGameState::IncreaseCurrentLevel()
{
	CurrentLevel++;
}

void AECGameState::ResetLevel()
{
	CurrentLevel = 1;
}
