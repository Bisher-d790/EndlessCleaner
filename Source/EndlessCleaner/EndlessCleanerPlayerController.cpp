// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessCleanerPlayerController.h"
#include "EndlessCleanerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "InGameUIWidget.h"


AEndlessCleanerPlayerController::AEndlessCleanerPlayerController()
{
	// Setup variable values
	MaxLockMovementTime = 0.2f;
	InitJumpTime = 1.2f;
}

void AEndlessCleanerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set variables values
	CoinsCollected = 0;
	CurrentDistance = 0;

	// Stop movement
	bCanMove = false;
	bIsRunning = false;
	bLockMovement = true;

	// Start in the middle
	CurrentLane = 1;

	// Set the Player reference
	PlayerRef = Cast<AEndlessCleanerCharacter>(GetPawn());

	if (InGameUIWidgetClass)
	{
		InGameUIWidgetInstance = CreateWidget<UInGameUIWidget>(this, InGameUIWidgetClass);
	}

	if (InGameUIWidgetInstance)
	{
		InGameUIWidgetInstance->AddToViewport();

		InGameUIWidgetInstance->UpdateCoins(CoinsCollected);
		InGameUIWidgetInstance->UpdateDistance(CurrentDistance);
		InGameUIWidgetInstance->UpdateLives(CurrentLives);
	}
}

void AEndlessCleanerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveToSide", this, &AEndlessCleanerPlayerController::MoveToSide);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AEndlessCleanerPlayerController::Jump);
}

void AEndlessCleanerPlayerController::StartRunning()
{
	// Start movement
	bCanMove = true;
	bIsRunning = true;
	bLockMovement = false;

	PlayerRef->SetIsMoving(true);
}

void AEndlessCleanerPlayerController::StopRunning()
{
	// Stop movement
	PlayerRef->SetIsMoving(false);

	bCanMove = false;
	bIsRunning = false;
	bLockMovement = true;
}

void AEndlessCleanerPlayerController::SetInitialLives(int32 InitialLives)
{
	CurrentLives = InitialLives;
}

void AEndlessCleanerPlayerController::LoseLife(bool& bIsLastLife)
{
	bIsLastLife = false;
	if ((CurrentLives - 1) < 0)
	{
		CurrentLives = 0;
		bIsLastLife = true;
	}
	else
	{
		CurrentLives -= 1;
	}

	if (InGameUIWidgetInstance)
	{
		InGameUIWidgetInstance->UpdateLives(CurrentLives);
	}
}

void AEndlessCleanerPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// Lock Input when moving 
	if (bLockMovement)
	{
		CurrentLockMovementTime += DeltaTime;

		if (CurrentLockMovementTime >= MaxLockMovementTime)
		{
			CurrentLockMovementTime = 0.0f;
			bLockMovement = false;
		}
	}

	// If cannot move, return
	if (!bCanMove) return;

	if (bIsRunning)
	{
		CurrentDistance += DeltaTime;

		if (InGameUIWidgetInstance)
		{
			InGameUIWidgetInstance->UpdateDistance(CurrentDistance);
		}
	}

	// Stop Jump animation after jump time finishes
	if (bIsJumping)
	{
		RemainingJumpTime -= DeltaTime;

		if (RemainingJumpTime <= 0.0f)
		{
			bIsJumping = false;

			if (PlayerRef != nullptr)
			{
				PlayerRef->StopJumping();
			}
		}
	}
}

void AEndlessCleanerPlayerController::MoveToSide(float Value)
{
	if (!bCanMove || bLockMovement || !PlayerRef) return;

	// Move Right
	if (Value > 0.0f)
	{
		if (CurrentLane + 1 < 3)
		{
			bLockMovement = true;
			CurrentLockMovementTime = 0.0f;

			CurrentLane += 1;

			PlayerRef->MoveRight();
			CurrentLane = FMath::Clamp(CurrentLane, 0, 2);
		}
	}

	// Move Left
	if (Value < 0.0f)
	{
		if (CurrentLane - 1 >= 0)
		{
			bLockMovement = true;
			CurrentLockMovementTime = 0.0f;

			PlayerRef->MoveLeft();

			CurrentLane -= 1;
			CurrentLane = FMath::Clamp(CurrentLane, 0, 2);
		}
	}
}

void AEndlessCleanerPlayerController::Jump()
{
	if (!bCanMove || bIsJumping || !PlayerRef) return;

	//UE_LOG(LogTemp, Warning, TEXT("Jump."));

	bIsJumping = true;

	RemainingJumpTime = InitJumpTime;

	PlayerRef->Jump();
}

void AEndlessCleanerPlayerController::Respawn()
{
	CurrentLane = 1; // Set to middle
	bIsRunning = false;
	CurrentDistance = 0.0f;

	if (InGameUIWidgetInstance)
	{
		InGameUIWidgetInstance->UpdateCoins(CoinsCollected);
		InGameUIWidgetInstance->UpdateDistance(CurrentDistance);
	}

	bIsRunning = false;
}

void AEndlessCleanerPlayerController::OnCollectCoin()
{
	CoinsCollected += 1;

	if (InGameUIWidgetInstance)
	{
		InGameUIWidgetInstance->UpdateCoins(CoinsCollected);
	}
}