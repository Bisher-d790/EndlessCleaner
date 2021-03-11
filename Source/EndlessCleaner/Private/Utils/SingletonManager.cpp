// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/SingletonManager.h"
#include "Core/EndlessCleanerGameController.h"

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