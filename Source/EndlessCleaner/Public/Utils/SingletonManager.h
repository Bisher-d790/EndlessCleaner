// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/SingletonAdapter.h"
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
	UFUNCTION(BlueprintCallable)
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
	class TArray<TSharedPtr<class SingletonAdapter>> SingletonInstances;

	template<typename T>
	int8 GetIndexByInstanceType()
	{
		int Index = -1;

		for (int InstanceIndex = 0; InstanceIndex < SingletonInstances.Num(); InstanceIndex++)
		{
			if (SingletonInstances[InstanceIndex].IsValid()
				&& IsValid(SingletonInstances[InstanceIndex]->GetWrappedObject())
				&& Cast<T>(SingletonInstances[InstanceIndex]->GetWrappedObject()))
			{
				Index = InstanceIndex;
				break;
			}
		}

		return Index;
	};

public:
	template<typename T>
	T* GetSingletonInstance(bool bSpawnIfNoInstanceFound = false)
	{
		int Index = GetIndexByInstanceType<T>();

		T* Instance = nullptr;

		// No Instance was found
		if (Index < 0)
		{
			if (bSpawnIfNoInstanceFound)
			{
				UWorld* World;
				//World = GEngine->GameViewport->GetWorld();
				World = GetWorld();

				Instance = World->SpawnActor<T>();
				SetSingletonInstance<T>(Instance);
			}
			else
			{
				return nullptr;
			}
		}
		// Instance found, Get the pointer to that object
		else
		{
			Instance = Cast<T>(SingletonInstances[Index]->GetWrappedObject());
		}

		return Instance;
	};

	template<typename T>
	int8 SetSingletonInstance(T* Instance)
	{
		if (!IsValid(Instance)) return -1;

		int Index = GetIndexByInstanceType<T>();

		// If there's no valid instance
		if (Index < 0 || !SingletonInstances[Index].IsValid())
		{
			TSharedPtr<SingletonAdapter> NewPointer = TSharedPtr<SingletonAdapter>(new SingletonAdapter(Instance));

			if (Index >= 0)
				// Replace the current instance
				SingletonInstances[Index] = NewPointer;
			else
			{
				SingletonInstances.Add(NewPointer);
				Index = SingletonInstances.Num() - 1;
			}
		}
		// If there's a valid instance
		else
		{
			Instance->Destroy();
		}

		return Index;
	};
};
