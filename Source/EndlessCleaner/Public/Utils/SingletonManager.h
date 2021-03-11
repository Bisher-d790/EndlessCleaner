// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
		static USingletonManager* GetInstance();

private:
	// A Generic Function to set the local Singleton Instances, that could be used for all Singleton Types
	// The caller Functions should be local methods, which are called from their respective type classes when an object is initialized
	template<typename T>
	void SetGenericSingletonInstance(T* Instance, T* SingletonInstance);

#pragma region GameController Instance
public:
	FORCEINLINE class AEndlessCleanerGameController* GetGameControllerInstance() { return GameControllerInstance; };

	void SetGameControllerInstance(class AEndlessCleanerGameController* Instance)
	{
		SetGenericSingletonInstance<AEndlessCleanerGameController>(Instance, GameControllerInstance);
	};

private:
	class AEndlessCleanerGameController* GameControllerInstance = nullptr;
#pragma endregion GameController Instance
};
