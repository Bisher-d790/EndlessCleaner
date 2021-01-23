// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EndlessCleanerPlayerController.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSCLEANER_API AEndlessCleanerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEndlessCleanerPlayerController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	// Called every frame
	virtual void PlayerTick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "Input")
		void MoveToSide(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
		void Jump();

	// Player Reference
	class AEndlessCleanerCharacter* PlayerRef;

	bool bLockMovement;

	float CurrentLockMovementTime;

	UPROPERTY(EditAnywhere, Category = Movement)
		float MaxLockMovementTime;

	bool bCanMove = false;

	UPROPERTY(EditAnywhere, Category = Movement)
		float InitJumpTime;

private:

	bool bIsRunning = false;

	int CurrentLane;

	bool bIsJumping;

	float RemainingJumpTime;

public:

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void StopRunning();
};