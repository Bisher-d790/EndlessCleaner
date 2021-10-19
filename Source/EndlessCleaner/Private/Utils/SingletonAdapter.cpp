// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/SingletonAdapter.h"

SingletonAdapter::SingletonAdapter(AActor* WrappedObject)
{
	ReferencedObject = WrappedObject;
}

SingletonAdapter::~SingletonAdapter()
{
	ReferencedObject->Destroy();
}
