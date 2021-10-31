// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Utils/SingletonManager.h"
#include "Player/EndlessCleanerCharacter.h"

#pragma region Singleton
AAudioManager* AAudioManager::GetInstance()
{
	return USingletonManager::GetInstance()->GetSingletonInstance<AAudioManager>(StaticClass()->GetFName());
}

void AAudioManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set Singleton Instance
	USingletonManager::GetInstance()->SetSingletonInstance<AAudioManager>(this, StaticClass()->GetFName());
}

void AAudioManager::BeginDestroy()
{
	USingletonManager::GetInstance()->DeleteInstance(StaticClass()->GetFName());

	Super::BeginDestroy();
}
#pragma endregion Singleton

// Sets default values
AAudioManager::AAudioManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set initial value
	BackgroundMusicVolume = 1.0f;
	FootstepSFXVolume = 1.0f;
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<AEndlessCleanerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AAudioManager::PlayBackgroundMusic()
{
	if (IsValid(BackgroundMusicComponent))
	{
		if (!BackgroundMusicComponent->IsPlaying())
			BackgroundMusicComponent->Play();
	}
	else
	{
		if (BackgroundMusic)
		{
			BackgroundMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BackgroundMusic, BackgroundMusicVolume);
			PlayBackgroundMusic();
		}
	}
}

void AAudioManager::StopBackgroundMusic()
{
	if (IsValid(BackgroundMusicComponent) && BackgroundMusicComponent->IsPlaying())
		BackgroundMusicComponent->Stop();
}

void AAudioManager::PlayFoostepSFX()
{
	if (FootstepSFX)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSFX, PlayerRef->GetActorLocation(), FootstepSFXVolume);
}