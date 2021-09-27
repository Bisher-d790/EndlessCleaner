// Fill out your copyright notice in the Description page of Project Settings.
#include "Utils/SingletonManager.h"
#include "Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"


USingletonManager* USingletonManager::GetInstance()
{
	if (GEngine)
	{
		USingletonManager* Instance = Cast<USingletonManager>(GEngine->GameSingleton);
		return Instance;
	}
	return nullptr;
}

template<typename T>
void USingletonManager::SetGenericSingletonInstance(T* Instance, T* SingletonInstance)
{
	if (SingletonInstance == nullptr) SingletonInstance = Instance;
	else if (SingletonInstance != Instance)
	{
		Instance->Destroy();
	}
}

#pragma region Audio Manager
AAudioManager* USingletonManager::GetAudioManagerInstance()
{
	if (AudioManagerInstance == nullptr)
	{
		UWorld* World = GEngine->GameViewport->GetWorld();

		AudioManagerInstance = World->SpawnActor<AAudioManager>();
	}

	return AudioManagerInstance;
}

void USingletonManager::SetAudioManagerInstance(AAudioManager* Instance)
{
	// TODO: Change the way SetGenericSingletonInstance works to storing objects in a Map
	if (AudioManagerInstance == nullptr) AudioManagerInstance = Instance;
	else if (AudioManagerInstance != Instance)
	{
		Instance->Destroy();
	}
}
#pragma endregion Audio Manager