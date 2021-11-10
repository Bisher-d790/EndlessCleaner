// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class ENDLESSCLEANER_API EnemyFactory
{
public:
	EnemyFactory(class UWorld* World);

	~EnemyFactory();

	virtual class AEnemy* CreateEnemy(TSubclassOf<AEnemy> EnemyClass, FVector SpawnPosition, float EnemyStartSpeed, float EnemyActualSpeed);

	virtual void DestroyEnemy(int EnemyIndex);

	virtual void DestroyEnemy(class AEnemy* KilledEnemy);

	void DestroyAllEnemies();

	inline class AEnemy* GetEnemy(int EnemyIndex) { return EnemiesList[EnemyIndex]; }

	inline class TArray<class AEnemy*> GetEnemyList() { return EnemiesList; }

private:
	TArray <class AEnemy*> EnemiesList;

	class AActor* EnemiesContainerActor;

	class UWorld* WorldContext;
};