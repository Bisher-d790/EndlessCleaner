// A library file to store Enumeration Types, State Types and any other Static Definitions needed for the game

#pragma once

#include "CoreMinimal.h"

// An Enumerate for the types of Ground Platforms used for the game
UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	VE_ThreeLanes_Ground		UMETA(DisplayName = "3Lanes|Ground"),			// Type for normal ground
	VE_ThreeLanes_OneBridge		UMETA(DisplayName = "3Lanes|One Bridge"),		// Type for two connections of ground
	VE_ThreeLanes_TwoBridges	UMETA(DisplayName = "3Lanes|Two Bridges"),		// Type for one connection of ground
	VE_ThreeLanes_GroundGap		UMETA(DisplayName = "3Lanes|Ground With Gap"),	// Type for normal ground with gap
};

// An Enumerate for game states
UENUM(BlueprintType)
enum class EGameState : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_PreparePlatforms		UMETA(DisplayName = "PreparePlatforms"),
	VE_PrepareGame			UMETA(DisplayName = "PrepareGame"),
	VE_Running				UMETA(DisplayName = "Running"),
	VE_RemovePlatforms		UMETA(DisplayName = "RemovePlatforms"),
	VE_Respawn				UMETA(DisplayName = "Respawn"),
	VE_GameOver				UMETA(DisplayName = "GameOver")
};