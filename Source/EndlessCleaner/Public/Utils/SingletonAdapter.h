// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class ENDLESSCLEANER_API SingletonAdapter
{
public:
	SingletonAdapter(class AActor* WrappedObject, FName ClassName);

	~SingletonAdapter();

	FORCEINLINE class AActor* GetWrappedObject() { return ReferencedObject; };

	FORCEINLINE FName GetClassName() { return ClassName; };

	inline bool GetIsValid() { return IsValid(ReferencedObject) && !ReferencedObject->GetFName().IsNone(); };

private:
	class AActor* ReferencedObject;

	FName ClassName;
};
