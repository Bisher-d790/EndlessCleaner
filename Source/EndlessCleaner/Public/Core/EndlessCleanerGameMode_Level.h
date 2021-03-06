// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Utils/Definitions.h"
#include "EndlessCleanerGameMode_Level.generated.h"

/**
 *
 */
 // A struct to store the different platform types
USTRUCT(BlueprintType)
struct FPlatformOptions
{
	GENERATED_USTRUCT_BODY()

public:

	FPlatformOptions()
	{
		PlatformType = EPlatformType::VE_Ground;
		Probability = 50.f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		EPlatformType PlatformType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		TSubclassOf<class APlatformModule> PlatformClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		float Probability;
};

UCLASS()
class ENDLESSCLEANER_API AEndlessCleanerGameMode_Level : public AGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEndlessCleanerGameMode_Level();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeGame();

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TArray<FPlatformOptions> PlatformTable;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		bool bUseNormalGroundBetweenBridges;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int VisiblePlatformNumber;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int NumberOfInitialPlatforms;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int InitialPlayerLives;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float RespawnTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float SpawnStartTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class APlatformsContainer> PlatformsContainerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Scoring")
		float CoinsMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Scoring")
		float SpeedMultiplier;

	class APlatformsContainer* PlatformsContainerActor;

private:

	int PlatformCount;

	int PlatformTypesRandomCeil = 5;	// According to the number of Platform Ground Types, types_of_ground - 1

	class APlatformModule* PreviousPlatform;

	class APlatformModule* FirstPlatform;

	class APlatformModule* PlatformToCheck;

	class AEndlessCleanerPlayerController* PlayerController;

	class AEndlessCleanerCharacter* Player;

	void SpawnNewPlatform();

	TSubclassOf<class APlatformModule> GetPlatformToSpawn(TArray<EPlatformType> PlatformTypeFilters = {});

	UFUNCTION()
		void OnRespawn();

	FTimerHandle RespawnTimerHandle;

public:

	EGameState GameState = EGameState::VE_None;

	UFUNCTION(BlueprintCallable, Category = "Death")
		void OnTriggerDeathActor();

	UFUNCTION(BlueprintCallable, Category = "Platforms")
		FORCEINLINE APlatformsContainer* GetPlatformsContainerActor() { return PlatformsContainerActor; };
};