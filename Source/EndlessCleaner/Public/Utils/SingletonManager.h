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


#pragma region Audio Manager
private:
	class AAudioManager* AudioManagerInstance = nullptr;

public:
	class AAudioManager* GetAudioManagerInstance();

	void SetAudioManagerInstance(AAudioManager* Instance);
#pragma endregion Audio Manager
};
