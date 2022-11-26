// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathBoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EndlessCleaner/Core/ECGameMode_Level.h"
#include "EndlessCleaner/Utils/Debug.h"


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
	if (bHasBeenTriggered) return;

	bHasBeenTriggered = true;

	PrintDebugLog(TEXT("Death Overlap"));

	Cast<AECGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()))->OnTriggerDeathActor();
}