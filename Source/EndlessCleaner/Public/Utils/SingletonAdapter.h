// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class ENDLESSCLEANER_API SingletonAdapter
{
public:
	SingletonAdapter(class AActor* WrappedObject);

	~SingletonAdapter();

	FORCEINLINE class AActor* GetWrappedObject() { return ReferencedObject; };

	FORCEINLINE void SetWrappedObject(class AActor* WrappedObject) { ReferencedObject = WrappedObject; };

private:
	class AActor* ReferencedObject;
};
