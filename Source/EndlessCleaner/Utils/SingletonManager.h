// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SingletonManager.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSCLEANER_API USingletonManager : public UObject
{
	GENERATED_BODY()

public:
	// Function to get reference to the Singleton Manager
	UFUNCTION(BlueprintPure)
		static USingletonManager* GetInstance()
	{
		if (GEngine)
		{
			USingletonManager* Instance = Cast<USingletonManager>(GEngine->GameSingleton);
			return Instance;
		}
		return nullptr;
	};

private:
	TMap<FName, AActor*> SingletonInstances;

	inline bool GetIsValid(AActor* Actor) { return IsValid(Actor) && !Actor->GetFName().IsNone(); };

	void CheckReferencesValidity()
	{
		for (auto& Instance : SingletonInstances)
		{
			if (!GetIsValid(Instance.Value))
			{
				SingletonInstances.Remove(Instance.Key);
			}
		}
	}

public:
	template<typename T>
	void SetSingletonInstance(T* Instance, FName ClassName)
	{
		if (!IsValid(Instance)) return;

		CheckReferencesValidity();

		// If there's no valid instance
		if (!SingletonInstances.Contains(ClassName))
		{
			SingletonInstances.Add(ClassName, Instance);
		}
		// If there's a valid instance
		else
		{
			Instance->Destroy();
		}
	};

	template<typename T>
	T* GetSingletonInstance(FName ClassName)
	{
		CheckReferencesValidity();

		T* Instance = nullptr;

		// No Instance was found
		if (!SingletonInstances.Contains(ClassName))
		{
			return nullptr;
		}
		// Instance found, Get the pointer to that object
		else
		{
			Instance = Cast<T>(*SingletonInstances.Find(ClassName));
		}

		return Instance;
	};

	void DeleteInstance(FName ClassName)
	{
		SingletonInstances.Remove(ClassName);
	}
};