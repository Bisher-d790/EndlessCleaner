// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/EnemyFactory.h"
#include "Gameplay/Enemy.h"


EnemyFactory::EnemyFactory(UWorld* World)
{
	WorldContext = World;

	EnemiesContainerActor = WorldContext->SpawnActor<AActor>();
#if WITH_EDITOR
	EnemiesContainerActor->SetActorLabel(TEXT("EnemiesContainer"));
#endif
	EnemiesContainerActor->AddComponentByClass(USceneComponent::StaticClass(), false, FTransform(), false);
}

EnemyFactory::~EnemyFactory()
{
	DestroyAllEnemies();
}

AEnemy* EnemyFactory::CreateEnemy(TSubclassOf<AEnemy> EnemyClass, FVector SpawnPosition, float EnemyStartSpeed, float EnemyActualSpeed)
{
	AEnemy* SpawnedEnemy = WorldContext->SpawnActor<AEnemy>(EnemyClass, SpawnPosition, FRotator::ZeroRotator);
	SpawnedEnemy->AttachToActor(EnemiesContainerActor, FAttachmentTransformRules::KeepWorldTransform);
	EnemiesList.Add(SpawnedEnemy);

	return SpawnedEnemy;
}

void EnemyFactory::DestroyEnemy(int EnemyIndex)
{
	if (EnemyIndex >= 0 && EnemyIndex < EnemiesList.Num() && IsValid(EnemiesList[EnemyIndex]))
		EnemiesList[EnemyIndex]->Destroy();

	EnemiesList.RemoveAt(EnemyIndex);
}

void EnemyFactory::DestroyEnemy(AEnemy* KilledEnemy)
{
	EnemiesList.Remove(KilledEnemy);

	if (IsValid(KilledEnemy)) KilledEnemy->Destroy();
}

void EnemyFactory::DestroyAllEnemies()
{
	for (int EnemyIndex = 0; EnemyIndex < EnemiesList.Num(); EnemyIndex++)
	{
		DestroyEnemy(EnemyIndex);
	}
}
