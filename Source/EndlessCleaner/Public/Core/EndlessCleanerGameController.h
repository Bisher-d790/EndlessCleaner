// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Definitions.h"
#include "EndlessCleanerGameController.generated.h"

// A struct to store the different platform types
USTRUCT(BlueprintType)
struct FProbabilityTable
{
	GENERATED_USTRUCT_BODY()

public:

	FProbabilityTable()
	{
		PlatformGroundType = EPlatformGroundType::VE_Ground;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		EPlatformGroundType PlatformGroundType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		TSubclassOf<class APlatformModule> PlatformClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		float Probability = 50.f;
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

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeGame();

	UPROPERTY(EditInstanceOnly, Category = "Settings")
		AActor* DeathGround;

	UPROPERTY(EditInstanceOnly, Category = "Settings")
		TArray<AActor*> EnvironmentalActorsToMove;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TArray<FProbabilityTable> PlatformTable;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int VisiblePlatformNumber;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int NumberOfInitialPlatforms;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		int PlayerLives;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float RespawnTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		float SpawnStartTimer;

	UPROPERTY(EditInstanceOnly, Category = "Settings")
		AActor* PlatformsParentActor;

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

	EPlatformGroundType GetPlatformTypeToSpawn(const EPlatformGroundType& PlatformType);

	TSubclassOf<class APlatformModule> GetPlatformModuleByType(const EPlatformGroundType& PreviousType);

	UFUNCTION()
		void OnRespawn();

	FTimerHandle RespawnTimerHandle;
};
