// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessCleanerPlayerController.h"
#include "EndlessCleanerCharacter.h"


AEndlessCleanerPlayerController::AEndlessCleanerPlayerController()
{
	// Setup variable values
	MaxLockMovementTime = 0.2f;
	InitJumpTime = 1.2f;
}

void AEndlessCleanerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Stop movement
	bCanMove = false;
	bIsRunning = false;
	bLockMovement = true;

	// Start in the middle
	CurrentLane = 1;

	// Set the Player reference
	PlayerRef = Cast<AEndlessCleanerCharacter>(GetPawn());

	// Start running in beginning
	StartRunning();
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

	if (PlayerRef != nullptr)
	{
		PlayerRef->SetIsMoving(true);
	}
}

void AEndlessCleanerPlayerController::StopRunning()
{
	// Stop movement
	if (PlayerRef != nullptr)
	{
		PlayerRef->SetIsMoving(false);
	}

	bCanMove = false;
	bIsRunning = false;
	bLockMovement = true;
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

	// Stop Jump animation after jump time finishes
	if (bIsJumping && bCanMove)
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
}