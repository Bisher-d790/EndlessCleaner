// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/SingletonManager.h"

USingletonManager* USingletonManager::GetInstance()
{
	if (GEngine)
	{
		USingletonManager* Instance = Cast<USingletonManager>(GEngine->GameSingleton);
		return Instance;
	}
	return nullptr;
}