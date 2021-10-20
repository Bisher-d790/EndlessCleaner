// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/SingletonAdapter.h"

SingletonAdapter::SingletonAdapter(AActor* WrappedObject, FName ClassName)
{
	ReferencedObject = WrappedObject;
	this->ClassName = ClassName;
}

SingletonAdapter::~SingletonAdapter()
{
	if (GetIsValid())
		ReferencedObject->Destroy();
}
