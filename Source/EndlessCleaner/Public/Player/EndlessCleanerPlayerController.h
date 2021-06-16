// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EndlessCleanerPlayerController.generated.h"

// Declare any touch gesture as an event here
#pragma region Touch Gestures Events
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSwipeHorizental, float, Value);
DECLARE_DELEGATE_OneParam(FOnSwipeVertical, float);
#pragma endregion Touch Gestures Events

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

	UFUNCTION(BlueprintCallable, Category = "Input")
		void Slide();

	void OnTouchBegin(const ETouchIndex::Type TouchIndex, const FVector Position);

	void OnTouchEnd(const ETouchIndex::Type TouchIndex, const FVector Position);

	FVector TouchBeginPosition = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		float TouchSwipeMinLength;

	// Touch Events variable declaration
	FOnSwipeHorizental OnSwipeHorizental;
	FOnSwipeVertical OnSwipeVertical;

	// Player Reference
	class AEndlessCleanerCharacter* PlayerRef;

	bool bLockMovement;

	float CurrentLockMovementTime;

	UPROPERTY(EditAnywhere, Category = Movement)
		float MaxLockMovementTime;

	bool bCanMove = false;

	UPROPERTY(EditAnywhere, Category = Movement)
		float LocationCorrectionDuration;

	UPROPERTY(EditAnywhere, Category = Movement)
		float JumpDuration;

	UPROPERTY(EditAnywhere, Category = Movement)
		float SlideDuration;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
		class UInGameUIWidget* InGameUIWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> InGameUIWidgetClass;

private:

	bool bIsRunning = false;

	int CurrentLane;

	float CurrentDistance;

	float CurrentTime;

	bool bIsJumping = false;

	float RemainingJumpTime;

	bool bIsSliding = false;

	float RemainingSlideTime;

	int CoinsCollected;

	int CurrentLives;

	bool bIsMovingLeft = false;

	bool bIsMovingRight = false;

	bool bCheckPosition = false;

	class APlatformModule* CurrentPlatform;

public:

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void StopRunning();

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void Respawn();

	void LoseLife(bool& isLastLive);

	void SetInitialLives(int32 InitialLives);

	void AddToCurrentDistance(float Distance);

	FORCEINLINE void SetCurrentPlatform(APlatformModule* Platform) { CurrentPlatform = Platform; };

	UFUNCTION(BlueprintCallable, Category = "Player Events")
		void OnCollectCoin();

	FORCEINLINE UInGameUIWidget* GetUI() { return InGameUIWidgetInstance; }

	FORCEINLINE int GetCoinsCollected() { return CoinsCollected; }

	FORCEINLINE int GetCurrentLives() { return CurrentLives; }

	FORCEINLINE float GetCurrentDistance() { return CurrentDistance; }

	FORCEINLINE float GetCurrentTime() { return CurrentTime; }

	UFUNCTION(BlueprintCallable, Category = Movement)
		FORCEINLINE bool GetIsRunning() { return bIsRunning; }

	UFUNCTION(BlueprintCallable, Category = Movement)
		FORCEINLINE bool GetIsJumping() { return bIsJumping; }

	UFUNCTION(BlueprintCallable, Category = Movement)
		FORCEINLINE bool GetIsSliding() { return bIsSliding; }
};