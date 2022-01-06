// A library file to store Enumeration Types, State Types and any other Static Definitions needed for the game

#pragma once

#include "CoreMinimal.h"

// An Enumerate for the types of Ground Platforms used for the game
UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	VE_Ground		UMETA(DisplayName = "Ground"),			// Type for normal ground
	VE_OneBridge	UMETA(DisplayName = "One Bridge"),		// Type for two connections of ground
	VE_TwoBridges	UMETA(DisplayName = "Two Bridges"),		// Type for one connection of ground
	VE_GroundGap	UMETA(DisplayName = "Ground With Gap"),	// Type for normal ground with gap
};

// An Enumerate for game states
UENUM(BlueprintType)
enum class EGameplayState : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_PreparingPlatforms	UMETA(DisplayName = "PreparingPlatforms"),
	VE_PreparingGame		UMETA(DisplayName = "PreparingGame"),
	VE_Running				UMETA(DisplayName = "Running"),
	VE_RemovingPlatforms	UMETA(DisplayName = "RemovingPlatforms"),
	VE_Respawning			UMETA(DisplayName = "Respawning"),
	VE_GameOver				UMETA(DisplayName = "GameOver")
};