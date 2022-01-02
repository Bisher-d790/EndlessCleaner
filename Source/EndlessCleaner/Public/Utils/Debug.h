// A library file to store simple utilities needed for the game

#pragma once

#include "CoreMinimal.h"


#pragma region Logging
static bool ConsoleLogState = true;
static bool ScreenLogState = true;

static void SetDebugLogState(bool State)
{
	ConsoleLogState = State;

	FString Command = "Log LogTemp ";

	if (State)
		Command += "Log";
	else
		Command += "Off";

	GEngine->Exec(nullptr, *Command);
}

static void SetDebugScreenLogState(bool State)
{
	ScreenLogState = State;

	GEngine->bEnableOnScreenDebugMessages = State;
}

static void PrintDebugLog(FString Log, bool bPrintInConsole = true, bool bPrintOnScreen = true, FColor Color = FColor::Red, bool bForceLog = false)
{
	bool OriginalLogState = ConsoleLogState;
	bool OriginalScreenState = ScreenLogState;
	if (bForceLog)
	{
		if (bPrintInConsole && !ConsoleLogState)
			SetDebugLogState(true);

		if (bPrintInConsole && !ScreenLogState)
			SetDebugScreenLogState(true);
	}

	if (bPrintInConsole && ConsoleLogState)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Log);
	}

	if (bPrintOnScreen && ScreenLogState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, Color, Log);
	}

	if (OriginalLogState != ConsoleLogState)
		SetDebugLogState(OriginalLogState);

	if (OriginalScreenState != ScreenLogState)
		SetDebugScreenLogState(OriginalScreenState);
}
#pragma endregion