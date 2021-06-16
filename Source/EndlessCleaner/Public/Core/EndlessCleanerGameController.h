// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Definitions.h"
#include "EndlessCleanerGameController.generated.h"

// A struct to store the different platform types
USTRUCT(BlueprintType)
struct FPlatformOptions
{
	GENERATED_USTRUCT_BODY()

public:

	FPlatformOptions()
	{
		PlatformType = EPlatformType::VE_ThreeLanes_Ground;
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
class ENDLESSCLEANER_API AEndlessCleanerGameController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEndlessCleanerGameController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Singleton
public:
	// Static Function to get the class instance reference
	UFUNCTION(BlueprintCallable)
		static AEndlessCleanerGameController* GetInstance();

private:
	// Function to be called on initializing an object to set the Singleton reference
	void SetInstance();
#pragma endregion Singleton

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeGame();

	UPROPERTY(EditInstanceOnly, Category = "Settings")
		AActor* DeathGround;

	UPROPERTY(EditInstanceOnly, Category = "Settings")
		TArray<AActor*> EnvironmentalActorsToMove;

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

	UPROPERTY(EditInstanceOnly, Category = "Settings")
		AActor* PlatformsParentActor;

	UPROPERTY(EditDefaultsOnly, Category = "Scoring")
		float LivesMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Scoring")
		float CoinsMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Scoring")
		float SpeedMultiplier;

public:

	EGameState GameState = EGameState::VE_None;

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
};
