// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EndlessCleanerPlayerController.h"
#include "Player/EndlessCleanerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UI/InGameUIWidget.h"
#include "Gameplay/Platforms/PlatformModule.h"
#include "Gameplay/Platforms/PlatformsContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Core/EndlessCleanerGameMode_Level.h"
#include "Utils/Utils.h"


AEndlessCleanerPlayerController::AEndlessCleanerPlayerController()
{
	// Setup variable values
	MaxLockMovementTime = 0.2f;
	JumpDuration = 0.85f;
	SlideDuration = 1.f;
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

	if (IsValid(InGameUIWidgetClass))
	{
		InGameUIWidgetInstance = CreateWidget<UInGameUIWidget>(this, InGameUIWidgetClass);
	}

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->AddToViewport();

		InGameUIWidgetInstance->UpdateCoins(CoinsCollected);
		InGameUIWidgetInstance->UpdateDistance(CurrentDistance);
		InGameUIWidgetInstance->UpdateLives(CurrentLives);
		InGameUIWidgetInstance->UpdateTime(CurrentTime);
		int Level = Cast<AEndlessCleanerGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()))->GetCurrentLevel();
		InGameUIWidgetInstance->UpdateLevel(Level);
	}
}

void AEndlessCleanerPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!IsValid(PlatformsContainer))
		PlatformsContainer = Cast<AEndlessCleanerGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()))->GetPlatformsContainerActor();

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

		if (IsValid(InGameUIWidgetInstance))
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

			if (IsValid(PlayerRef))
			{
				PlayerRef->StopJumping();
			}
		}
	}

	if (bIsMovingLeft || bIsMovingRight)
	{
		FVector ActorLocation = PlayerRef->GetActorLocation();

		TArray<FLaneOptions> Lanes = CurrentPlatform->GetLanesArray();
		for (int LaneIndex = 0; LaneIndex < Lanes.Num(); LaneIndex++)
		{
			int PreviousLane = -1;
			if (bIsMovingLeft)
			{
				PreviousLane = CurrentLane + 1 >= Lanes.Num() ? 0 : CurrentLane + 1;
			}
			else
			{
				PreviousLane = CurrentLane - 1 < 0 ? Lanes.Num() - 1 : CurrentLane - 1;
			}

			if (LaneIndex == PreviousLane) continue;
			else
			{
				FLaneOptions Lane = CurrentPlatform->GetLanesArray()[CurrentLane];

				float CurrentRotationAngle = PlatformsContainer->GetActorRotation().GetDenormalized().Roll;
				float TargetRotationAngle = Lane.LaneAngle;

				if (TargetRotationAngle == 0.0f || TargetRotationAngle == 360.0f)
					TargetRotationAngle = CurrentRotationAngle > 300 ? 360.0f : 0.0f;

				PrintDebugLog(FString::Printf(TEXT("Current Angle: %.2f, Target Angle: %.2f"), CurrentRotationAngle, TargetRotationAngle));
				PrintDebugLog(FString::Printf(TEXT("CurrentLane: %d, PreviousLane: %d"), CurrentLane, PreviousLane));

				if (FMath::IsNearlyEqual(CurrentRotationAngle, TargetRotationAngle, Lane.LaneWidthAngle))
				{
					// If not jumping, don't stop moving to side
					if (!bIsJumping)
					{
						PlayerRef->StopMoveToSide();
						PlatformsContainer->StopRotation();

						FRotator Rotation = (FRotator(0.f, 0.f, Lane.LaneAngle)).GetNormalized();
						PlatformsContainer->SetActorRotation(Rotation);

						bIsMovingLeft = false;
						bIsMovingRight = false;
						break;
					}

					// set the new current lane
					if (bIsMovingLeft)
					{
						CurrentLane = CurrentLane - 1 < 0 ? Lanes.Num() - 1 : CurrentLane - 1;
					}
					else
					{
						CurrentLane = CurrentLane + 1 >= Lanes.Num() ? 0 : CurrentLane + 1;
					}
					PrintDebugLog(FString::Printf(TEXT("Current Lane: %i"), CurrentLane));
				}
			}
		}
	}

	if (IsValid(CurrentPlatform) && bCorrectPostition)
	{
		// Constrain the player to the lane's Y position
		FVector PlayerLocation = PlayerRef->GetActorLocation();
		FVector TargetLocation = FVector::ZeroVector;
		//FVector TargetLocation = CurrentPlatform->GetLanesArray()[CurrentLane].LanePosition;

		// Don't correct on X, Z axis
		TargetLocation.X = PlayerLocation.X;
		TargetLocation.Z = PlayerLocation.Z;

		if (!PlayerLocation.Equals(TargetLocation, CurrentPlatform->GetLanesArray()[CurrentLane].LaneWidth))
		{
			PrintDebugLog(FString::Printf(TEXT("TargetLocation.Y: %.2f, PlayerRef.Y: %.2f"), PlayerLocation.Y, PlayerRef->GetActorLocation().Y));

			FVector Newlocation = TargetLocation;

			PlayerRef->SetActorLocation(Newlocation);
		}
	}

	// Stop Slide animation after slide time finishes
	if (bIsSliding)
	{
		RemainingSlideTime -= DeltaTime;

		if (RemainingSlideTime <= 0.0f)
		{
			bIsSliding = false;

			if (IsValid(PlayerRef))
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
			PrintDebugLog(TEXT("Swipe Left."));
			OnSwipeHorizental.Execute(-1.0f);
		}
		else // Swipe Right
		{
			PrintDebugLog(TEXT("Swipe Right."));
			OnSwipeHorizental.Execute(1.0f);
		}
	}
	else // Vertical Swipe
	{
		if (TouchBeginPosition.Y > Position.Y) // Swipe Up
		{
			PrintDebugLog(TEXT("Swipe Up."));
			OnSwipeVertical.Execute(1.0f);
		}
		else // Swipe Down
		{
			PrintDebugLog(TEXT("Swipe Down."));
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

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->UpdateLives(CurrentLives);
	}
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

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->UpdateLives(CurrentLives);
	}
}

void AEndlessCleanerPlayerController::MoveToSide(float Value)
{
	if (!bCanMove || bLockMovement || !IsValid(PlayerRef) || !IsValid(PlatformsContainer) || !IsValid(CurrentPlatform)) return;

	if (FMath::IsNearlyZero(Value)) return;

	if ((Value < 0.0f && bIsMovingLeft) || (Value > 0.0f && bIsMovingRight)) return;

	int LanesCount = CurrentPlatform->GetLanesArray().Num();

	bLockMovement = true;
	CurrentLockMovementTime = 0.0f;

	PrintDebugLog(FString::Printf(TEXT("LanesCount: %d"), LanesCount));
	PrintDebugLog(FString::Printf(TEXT("Current Lane: %d"), CurrentLane));

	// Move Left
	if (Value < 0.0f)
	{
		if (CurrentLane - 1 >= 0)
			CurrentLane--;
		else
			CurrentLane = LanesCount - 1;

		bIsMovingLeft = true;
		PlatformsContainer->RotateRight();
		PlayerRef->MoveLeft();
	}
	// Move Right
	else
	{
		if (CurrentLane + 1 < LanesCount)
			CurrentLane++;
		else
			CurrentLane = 0;

		bIsMovingRight = true;
		PlatformsContainer->RotateLeft();
		PlayerRef->MoveRight();
	}

	PrintDebugLog(FString::Printf(TEXT("Next Lane: %d"), CurrentLane));
}

void AEndlessCleanerPlayerController::Jump()
{
	if (!bCanMove || bIsJumping || !IsValid(PlayerRef)) return;

	PrintDebugLog(TEXT("Jump."));

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
	if (!bCanMove || bIsSliding || !IsValid(PlayerRef)) return;

	PrintDebugLog(TEXT("Slide."));

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
	bIsMovingLeft = bIsMovingRight = false;
	PlayerRef->Respawn();

	bIsRunning = false;
}

void AEndlessCleanerPlayerController::OnCollectCoin()
{
	CoinsCollected += 1;

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->UpdateCoins(CoinsCollected);
	}
}

void AEndlessCleanerPlayerController::OnKillEnemy(AEnemy* KilledEnemy)
{
	PrintDebugLog("Enemy Killed");

	AEndlessCleanerGameMode_Level* GameMode = Cast<AEndlessCleanerGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnEnemyKilled(KilledEnemy);
	EnemiesKilled += 1;

	int Level = GameMode->GetCurrentLevel();

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->UpdateLevel(Level);
	}
}

void AEndlessCleanerPlayerController::AddToCurrentDistance(float Distance)
{
	if (bIsRunning)
	{
		CurrentDistance += Distance;

		if (IsValid(InGameUIWidgetInstance))
		{
			InGameUIWidgetInstance->UpdateDistance(CurrentDistance);
		}
	}
}