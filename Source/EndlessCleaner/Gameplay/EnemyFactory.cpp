// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyFactory.h"
#include "Enemy.h"
#include "Components/PerpetualMovementComponent.h"


TSharedPtr<EnemyFactory> EnemyFactory::Instance = nullptr;

EnemyFactory* EnemyFactory::GetInstance(UWorld* World)
{
	if (!Instance.IsValid() || Instance->WorldContext != World)
	{
		Instance.Reset();

		EnemyFactory* NewInstance = new EnemyFactory(World);
		Instance = MakeShareable(NewInstance);
	}

	return Instance.Get();
}

EnemyFactory::EnemyFactory(UWorld* World)
{
	WorldContext = World;

	EnemiesContainerActor = WorldContext->SpawnActor<AActor>();
#if WITH_EDITOR
	EnemiesContainerActor->SetActorLabel(TEXT("EnemiesContainer"));
#endif
	EnemiesContainerActor->AddComponentByClass(USceneComponent::StaticClass(), false, FTransform(), false);
}

AEnemy* EnemyFactory::CreateEnemy(TSubclassOf<AEnemy> EnemyClass, FVector SpawnPosition, float EnemyRetractionSpeed, float EnemyActualSpeed, float EnemyDistanceToRetract)
{
	AEnemy* SpawnedEnemy = WorldContext->SpawnActor<AEnemy>(EnemyClass, SpawnPosition, FRotator::ZeroRotator);
	SpawnedEnemy->AttachToActor(EnemiesContainerActor, FAttachmentTransformRules::KeepWorldTransform);
	SpawnedEnemy->PerpetualMovementComponent->SetVelocity(FVector(EnemyActualSpeed, 0.f, 0.f));
	SpawnedEnemy->PerpetualMovementComponent->SetRetractionVelocity(FVector(EnemyRetractionSpeed, 0.f, 0.f));
	SpawnedEnemy->PerpetualMovementComponent->SetDistanceToRetract(EnemyDistanceToRetract);

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