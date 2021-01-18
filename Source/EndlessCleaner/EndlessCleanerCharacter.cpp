// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessCleanerCharacter.h"

// Sets default values
AEndlessCleanerCharacter::AEndlessCleanerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEndlessCleanerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndlessCleanerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEndlessCleanerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

