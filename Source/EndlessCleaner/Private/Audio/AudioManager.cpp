// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Utils/SingletonManager.h"
#include "Player/EndlessCleanerCharacter.h"

#pragma region Singleton
AAudioManager* AAudioManager::GetInstance()
{
	return USingletonManager::GetInstance()->GetSingletonInstance<AAudioManager>();
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

void AAudioManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set Singleton Instance
	USingletonManager::GetInstance()->SetSingletonInstance<AAudioManager>(this);
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	if (BackgroundMusic)
		BackgroundMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BackgroundMusic, BackgroundMusicVolume);
	// Stop BG music by default
	StopBackgroundMusic();

	PlayerRef = Cast<AEndlessCleanerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AAudioManager::PlayBackgroundMusic()
{
	if (BackgroundMusicComponent)
		BackgroundMusicComponent->Play();
}

void AAudioManager::StopBackgroundMusic()
{
	if (BackgroundMusicComponent)
		BackgroundMusicComponent->Stop();
}

void AAudioManager::PlayFoostepSFX()
{
	if (FootstepSFX)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSFX, PlayerRef->GetActorLocation(), FootstepSFXVolume);
}