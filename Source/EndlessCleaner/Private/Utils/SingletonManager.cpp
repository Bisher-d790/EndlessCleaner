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


#pragma region Audio Manager
AAudioManager* USingletonManager::GetAudioManagerInstance()
{
	// Don't spawn instance if null
	/*if (AudioManagerInstance == nullptr)
	{
		UWorld* World = GEngine->GameViewport->GetWorld();

		AudioManagerInstance = World->SpawnActor<AAudioManager>();
	}*/

	return AudioManagerInstance;
}

void USingletonManager::SetAudioManagerInstance(AAudioManager* Instance)
{
	//if (AudioManagerInstance == nullptr) 
	AudioManagerInstance = Instance;
	/*else if (AudioManagerInstance != Instance)
	{
		Instance->Destroy();
	}*/
}
#pragma endregion Audio Manager