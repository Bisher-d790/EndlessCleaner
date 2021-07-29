// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DeathBoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/EndlessCleanerGameMode_Level.h"

// Sets default values for this component's properties
UDeathBoxComponent::UDeathBoxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	OnComponentBeginOverlap.AddDynamic(this, &UDeathBoxComponent::OnBeginOverlap);
}

void UDeathBoxComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AEndlessCleanerGameMode_Level::PrintDebugLog(TEXT("Death Overlap"));

	Cast<AEndlessCleanerGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()))->OnTriggerDeathActor();
}