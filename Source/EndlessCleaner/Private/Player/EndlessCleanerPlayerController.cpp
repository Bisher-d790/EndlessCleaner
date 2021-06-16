// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EndlessCleanerPlayerController.h"
#include "Player/EndlessCleanerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UI/InGameUIWidget.h"
#include "Gameplay/PlatformModule.h"


AEndlessCleanerPlayerController::AEndlessCleanerPlayerController()
{
	// Setup variable values
	MaxLockMovementTime = 0.2f;
	JumpDuration = 0.85f;
	SlideDuration = 1.f;
	LocationCorrectionDuration = 0.05;
}

void AEndlessCleanerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set variables values
	CoinsCollected = 0;
	CurrentDistance = 0;
	CurrentTime = 0;
	TouchSwipeMinLength = 100.f;

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
		InGameUIWidgetInstance->UpdateTime(CurrentTime);
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
		CurrentTime += DeltaTime;

		if (InGameUIWidgetInstance)
		{
			InGameUIWidgetInstance->UpdateTime(CurrentTime);
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
	else
	{
		if (bIsMovingLeft)
		{
			FVector ActorLocation = PlayerRef->GetActorLocation();

			TArray<FLaneOptions> Lanes = CurrentPlatform->GetLanesArray();
			for (int i = 0; i < Lanes.Num(); i++)
			{
				if (i == CurrentLane + 1) continue;
				else if (FMath::IsNearlyEqual(ActorLocation.Y, Lanes[i].LanePosition.Y, Lanes[i].LaneWidth))
				{
					PlayerRef->StopMoveToSide();

					CurrentLane = i;
					//UE_LOG(LogTemp, Warning, TEXT("Current Lane: %i"), CurrentLane);

					bCheckPosition = true;
					bIsMovingLeft = false;
					break;
				}
			}
		}
		else if (bIsMovingRight)
		{
			FVector ActorLocation = PlayerRef->GetActorLocation();

			TArray<FLaneOptions> Lanes = CurrentPlatform->GetLanesArray();
			for (int i = 0; i < Lanes.Num(); i++)
			{
				if (i == CurrentLane - 1) continue;
				else if (FMath::IsNearlyEqual(ActorLocation.Y, Lanes[i].LanePosition.Y, Lanes[i].LaneWidth))
				{
					PlayerRef->StopMoveToSide();

					CurrentLane = i;
					//UE_LOG(LogTemp, Warning, TEXT("Current Lane: %i"), CurrentLane);

					bCheckPosition = true;
					bIsMovingRight = false;
					break;
				}
			}
		}
		else if (bCheckPosition)
		{
			FVector TargetLocation = PlayerRef->GetActorLocation();
			FVector LaneLocation = CurrentPlatform->GetLanesArray()[CurrentLane].LanePosition;
			if (!FMath::IsNearlyEqual(TargetLocation.Y, LaneLocation.Y))
			{
				TargetLocation.Y = LaneLocation.Y;
				PlayerRef->SetActorLocation(FMath::Lerp(PlayerRef->GetActorLocation(), TargetLocation, LocationCorrectionDuration));
			}
		}
	}

	// Stop Slide animation after slide time finishes
	if (bIsSliding)
	{
		RemainingSlideTime -= DeltaTime;

		if (RemainingSlideTime <= 0.0f)
		{
			bIsSliding = false;

			if (PlayerRef != nullptr)
			{
				PlayerRef->UnCrouch();
			}
		}
	}
}

#pragma region Input Settings

void AEndlessCleanerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveToSide", this, &AEndlessCleanerPlayerController::MoveToSide);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AEndlessCleanerPlayerController::Jump);

	InputComponent->BindAction("Slide", IE_Pressed, this, &AEndlessCleanerPlayerController::Slide);

	// Touch Input Binding
	InputComponent->BindTouch(IE_Pressed, this, &AEndlessCleanerPlayerController::OnTouchBegin);

	InputComponent->BindTouch(IE_Released, this, &AEndlessCleanerPlayerController::OnTouchEnd);

	OnSwipeHorizental.BindDynamic(this, &AEndlessCleanerPlayerController::MoveToSide);

	// Bind Vertical Swipe, UP with Jump and DOWN with Slide
	OnSwipeVertical.BindLambda([this](float Value)
		{
			if (Value > 0) this->Jump();
			else this->Slide();
		});
}

void AEndlessCleanerPlayerController::OnTouchBegin(const ETouchIndex::Type TouchIndex, const FVector Position)
{
	// Disable multi-touch, no Touch Gestures requires multi-touching yet
	if (TouchIndex != ETouchIndex::Touch1)	return;

	TouchBeginPosition = Position;
}

void AEndlessCleanerPlayerController::OnTouchEnd(const ETouchIndex::Type TouchIndex, const FVector Position)
{
	// Disable multi-touch, no Touch Gestures requires multi-touching yet
	if (TouchIndex != ETouchIndex::Touch1)	return;

	// if the drag is longer than a specific length
	float TouchSwipeLength = FVector::Distance(Position, TouchBeginPosition);
	if (TouchSwipeLength < TouchSwipeMinLength) return;

	float XLength = FMath::Abs(Position.X - TouchBeginPosition.X);
	float YLength = FMath::Abs(Position.Y - TouchBeginPosition.Y);

	if (XLength > YLength) //Horizental Swipe
	{
		if (TouchBeginPosition.X > Position.X) // Swipe Left
		{
			//UE_LOG(LogTemp, Warning, TEXT("Swipe Left."));
			OnSwipeHorizental.Execute(-1.0f);
		}
		else // Swipe Right
		{
			//UE_LOG(LogTemp, Warning, TEXT("Swipe Right."));
			OnSwipeHorizental.Execute(1.0f);
		}
	}
	else // Vertical Swipe
	{
		if (TouchBeginPosition.Y > Position.Y) // Swipe Up
		{
			//UE_LOG(LogTemp, Warning, TEXT("Swipe Up."));
			OnSwipeVertical.Execute(1.0f);
		}
		else // Swipe Down
		{
			//UE_LOG(LogTemp, Warning, TEXT("Swipe Down."));
			OnSwipeVertical.Execute(-1.0f);
		}
	}
}

#pragma endregion Input Settings

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
	if ((CurrentLives - 1) <= 0)
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

void AEndlessCleanerPlayerController::MoveToSide(float Value)
{
	if (!bCanMove || bLockMovement || !PlayerRef) return;


	// Move Left
	if (Value < 0.0f)
	{
		if (CurrentLane - 1 >= 0)
		{
			bLockMovement = true;
			bIsMovingLeft = true;

			CurrentLockMovementTime = 0.0f;

			CurrentLane -= 1;
			CurrentLane = FMath::Clamp(CurrentLane, 0, 2);

			PlayerRef->MoveLeft();
		}
	}
	// Move Right
	else if (Value > 0.0f)
	{
		if (CurrentLane + 1 < 3)
		{
			bLockMovement = true;
			bIsMovingRight = true;

			CurrentLockMovementTime = 0.0f;

			CurrentLane += 1;
			CurrentLane = FMath::Clamp(CurrentLane, 0, 2);

			PlayerRef->MoveRight();
		}
	}

}

void AEndlessCleanerPlayerController::Jump()
{
	if (!bCanMove || bIsJumping || !PlayerRef) return;

	//UE_LOG(LogTemp, Warning, TEXT("Jump."));

	bIsJumping = true;

	RemainingJumpTime = JumpDuration;

	// if Sliding, stop sliding and continue to jump
	if (bIsSliding)
	{
		RemainingSlideTime = 0.0f;
		bIsSliding = false;
		PlayerRef->UnCrouch();

		// Jump in the next tick, to avoid stucking to the ground
		// Run Jump() concurrently in the next tick
		FTimerDelegate TimerCallback;
		AEndlessCleanerCharacter* PlayerTemp = PlayerRef;
		FTimerHandle Handle;

		TimerCallback.BindLambda([PlayerTemp]
			{
				// callback;
				PlayerTemp->Jump();
			});

		GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, GetWorld()->GetDeltaSeconds(), false);

		return;
	}

	PlayerRef->Jump();
}

void AEndlessCleanerPlayerController::Slide()
{
	if (!bCanMove || bIsSliding || !PlayerRef) return;

	//UE_LOG(LogTemp, Warning, TEXT("Slide."));

	bIsSliding = true;

	RemainingSlideTime = SlideDuration;

	// if Jumping, slide after finishing jump
	if (bIsJumping)
	{
		RemainingSlideTime += RemainingJumpTime;
	}

	PlayerRef->Crouch();
}

void AEndlessCleanerPlayerController::Respawn()
{
	CurrentLane = 1; // Set to middle
	bIsRunning = false;
	CurrentDistance = 0.0f;
	CurrentTime = 0.0f;
	bIsMovingLeft = bIsMovingRight = false;
	PlayerRef->Respawn();

	if (InGameUIWidgetInstance)
	{
		InGameUIWidgetInstance->UpdateCoins(CoinsCollected);
		InGameUIWidgetInstance->UpdateDistance(CurrentDistance);
		InGameUIWidgetInstance->UpdateTime(CurrentTime);
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

void AEndlessCleanerPlayerController::AddToCurrentDistance(float Distance)
{
	if (bIsRunning)
	{
		CurrentDistance += Distance;

		if (InGameUIWidgetInstance)
		{
			InGameUIWidgetInstance->UpdateDistance(CurrentDistance);
		}
	}
}