// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/Definitions.h"
#include "PlatformModule.generated.h"

UCLASS()
class ENDLESSCLEANER_API APlatformModule : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlatformModule();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
		class USceneComponent* StartModulePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
		class USceneComponent* EndModulePoint;

	UPROPERTY(EditDefaultsOnly, Category = "Platform Settings")
		EPlatformGroundType PlatformGroundType;

	UPROPERTY(EditDefaultsOnly, Category = "Platform Settings")
		float PlatformLength;

	APlatformModule* PreviousPlatform = nullptr;

	APlatformModule* NextPlatform = nullptr;

public:
	FORCEINLINE float GetPlatformLength() { return PlatformLength; };

	FORCEINLINE EPlatformGroundType GetPlatformType() { return PlatformGroundType; };

	FORCEINLINE APlatformModule* GetNextPlatform() { return NextPlatform; };

	FORCEINLINE void SetNextPlatform(APlatformModule* Value) { NextPlatform = Value; };

	FORCEINLINE APlatformModule* GetPreviousPlatform() { return PreviousPlatform; };

	FORCEINLINE void SetPreviousPlatform(APlatformModule* Value) { PreviousPlatform = Value; };

	void DestroyPlatform();
};
