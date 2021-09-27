// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


// Sets default values
AAudioManager::AAudioManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	BackgroundMusicComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BackgroundMusic);
	// Stop BG music by default
	StopBackgroundMusic();
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