// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"

UCLASS()
class ENDLESSCLEANER_API AAudioManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAudioManager();

#pragma region Singleton
public:
	static AAudioManager* GetInstance();
#pragma endregion Singleton

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		class USoundBase* BackgroundMusic;

	class UAudioComponent* BackgroundMusicComponent;

public:
	UFUNCTION(BlueprintCallable, Category = "Settings")
		void PlayBackgroundMusic();

	UFUNCTION(BlueprintCallable, Category = "Settings")
		void StopBackgroundMusic();
};
