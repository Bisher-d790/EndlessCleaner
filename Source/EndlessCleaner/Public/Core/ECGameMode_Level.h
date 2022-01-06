// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Utils/Definitions.h"
#include "ECGameMode_Level.generated.h"

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
class ENDLESSCLEANER_API AECGameMode_Level : public AGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AECGameMode_Level();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Called when the game starts or when spawned
	virtual void StartPlay() override;

	void InitializeGame();

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TArray<FPlatformOptions> PlatformTable;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		bool bInitializeFromPlayerStart;

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

	// Number of platforms whose pickups rush to you when you come near them
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int PickupsRushPlatformsCount;

	// Number of platforms whose Obstacles rush to you when you come near them
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int ObstaclesRushPlatformsCount;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class AEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float EnemyRetractionSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float EnemySpeedFirstLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float EnemySpeedLevelUp;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float EnemyKilledToLevelUp;

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

	class AECPlayerController* PlayerController;

	class AECCharacter* Player;

	class AECGameState* GameStateRef;

	void SpawnNewPlatform();

	TSubclassOf<class APlatformModule> GetPlatformToSpawn(TArray<EPlatformType> PlatformTypeFilters = {});

	UFUNCTION()
		void OnRespawn();

	FTimerHandle RespawnTimerHandle;

	class EnemyFactory* EnemyFactoryRef;

	void SpawnEnemy();

	void UpgradeLevel();

public:

	EGameplayState GameState = EGameplayState::VE_None;

	UFUNCTION(BlueprintCallable, Category = "Death")
		void OnTriggerDeathActor();

	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void OnEnemyKilled(class AEnemy* KilledEnemy);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
		int GetTotalEnemiesKilled();

	UFUNCTION(BlueprintCallable, Category = "Platforms")
		FORCEINLINE APlatformsContainer* GetPlatformsContainerActor() { return PlatformsContainerActor; };

	UFUNCTION(BlueprintCallable, Category = "Settings")
		FORCEINLINE int GetInitialPlayerLives() { return InitialPlayerLives; };

#pragma region Debug
private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
		bool bAllowDebugLogs = true;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
		bool bPrintLogsOnScreen = true;
#pragma endregion
};