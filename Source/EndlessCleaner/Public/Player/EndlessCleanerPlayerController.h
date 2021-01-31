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

	UPROPERTY(BlueprintReadWrite, Category = "UI")
		class UInGameUIWidget* InGameUIWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> InGameUIWidgetClass;

private:

	bool bIsRunning = false;

	int CurrentLane;

	float CurrentDistance;

	bool bIsJumping;

	float RemainingJumpTime;

	int CoinsCollected;

	int CurrentLives;

public:

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void StopRunning();

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void Respawn();

	void LoseLife(bool& isLastLive);

	void SetInitialLives(int32 InitialLives);

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void OnCollectCoin();

	FORCEINLINE UInGameUIWidget* GetUI() { return InGameUIWidgetInstance; }
};