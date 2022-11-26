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
	UFUNCTION(BlueprintCallable, Category = "Singleton")
		static AAudioManager* GetInstance();

	virtual void PostInitializeComponents() override;

	virtual void BeginDestroy() override;
#pragma endregion Singleton

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float BackgroundMusicVolume;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
		float FootstepSFXVolume;

	UPROPERTY(EditDefaultsOnly, Category = "Music")
		class USoundBase* BackgroundMusic;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
		class USoundBase* FootstepSFX;

	class UAudioComponent* BackgroundMusicComponent;

	class AECCharacter* PlayerRef;

public:
	UFUNCTION(BlueprintCallable, Category = "Music")
		void PlayBackgroundMusic();

	UFUNCTION(BlueprintCallable, Category = "Music")
		void StopBackgroundMusic();

	UFUNCTION(BlueprintCallable, Category = "SFX")
		void PlayFoostepSFX();
};
