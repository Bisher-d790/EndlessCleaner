// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ECPlayerController.h"
#include "Player/ECCharacter.h"
#include "Player/ECPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "UI/InGameUIWidget.h"
#include "Gameplay/Platforms/PlatformModule.h"
#include "Gameplay/Platforms/PlatformsContainer.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ECGameMode_Level.h"
#include "Core/ECGameState.h"
#include "Utils/Debug.h"


AECPlayerController::AECPlayerController()
{
	// Setup variable values
	MaxLockMovementTime = 0.2f;
	JumpDuration = 0.85f;
	SlideDuration = 1.f;
}

void AECPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set variables values
	TouchSwipeMinLength = 100.f;

	// Stop movement
	bCanMove = false;
	bIsRunning = false;
	bLockMovement = true;

	// Start in the middle
	CurrentLane = 1;

	// Set the Player reference
	PlayerRef = Cast<AECCharacter>(GetPawn());
	PlayerStateRef = Cast<AECPlayerState>(PlayerState);

	if (IsValid(InGameUIWidgetClass))
	{
		InGameUIWidgetInstance = CreateWidget<UInGameUIWidget>(this, InGameUIWidgetClass);
	}

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->AddToViewport();

		InGameUIWidgetInstance->UpdateCoins(PlayerStateRef->GetCoinsCollected());
		InGameUIWidgetInstance->UpdateDistance(PlayerStateRef->GetCurrentDistance());
		InGameUIWidgetInstance->UpdateLives(PlayerStateRef->GetCurrentLives());
		InGameUIWidgetInstance->UpdateTime(PlayerStateRef->GetCurrentTime());

		int Level = 1;
		AECGameState* GameState = GetWorld()->GetGameState<AECGameState>();
		if (GameState)	Level = GameState->GetCurrentLevel();

		InGameUIWidgetInstance->UpdateLevel(Level);
	}
}

void AECPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!IsValid(PlatformsContainer))
		PlatformsContainer = Cast<AECGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()))->GetPlatformsContainerActor();

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
		if (IsValid(InGameUIWidgetInstance))
		{
			InGameUIWidgetInstance->UpdateTime(PlayerStateRef->GetCurrentTime());
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
				// If not nearly equal, update the rotation rate (Accelerate)
				else
				{
					float RotationRate = GetCurrentRotationRate();

					if (bIsMovingLeft)
						PlatformsContainer->RotateRight(RotationRate);
					else
						PlatformsContainer->RotateLeft(RotationRate);
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

void AECPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveToSide", this, &AECPlayerController::MoveToSide);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AECPlayerController::Jump);

	InputComponent->BindAction("Slide", IE_Pressed, this, &AECPlayerController::Slide);

	// Touch Input Binding
	InputComponent->BindTouch(IE_Pressed, this, &AECPlayerController::OnTouchBegin);

	InputComponent->BindTouch(IE_Released, this, &AECPlayerController::OnTouchEnd);

	OnSwipeHorizental.BindDynamic(this, &AECPlayerController::MoveToSide);

	// Bind Vertical Swipe, UP with Jump and DOWN with Slide
	OnSwipeVertical.BindLambda([this](float Value)
		{
			if (Value > 0) this->Jump();
			else this->Slide();
		});
}

void AECPlayerController::OnTouchBegin(const ETouchIndex::Type TouchIndex, const FVector Position)
{
	// Disable multi-touch, no Touch Gestures requires multi-touching yet
	if (TouchIndex != ETouchIndex::Touch1)	return;

	TouchBeginPosition = Position;
}

void AECPlayerController::OnTouchEnd(const ETouchIndex::Type TouchIndex, const FVector Position)
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

void AECPlayerController::StartRunning()
{
	// Start movement
	bCanMove = true;
	bIsRunning = true;
	bLockMovement = false;

	PlayerRef->SetIsMoving(true);

	PlayerStateRef->StartTimer();
}

void AECPlayerController::StopRunning()
{
	// Stop movement
	PlayerRef->SetIsMoving(false);

	PlayerStateRef->StopTimer();

	bCanMove = false;
	bIsRunning = false;
	bLockMovement = true;
}

void AECPlayerController::SetInitialLives(int32 InitialLives)
{
	PlayerStateRef->SetCurrentLives(InitialLives);

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->UpdateLives(PlayerStateRef->GetCurrentLives());
	}
}

void AECPlayerController::LoseLife(bool& bIsLastLife)
{
	bIsLastLife = false;

	PlayerStateRef->DecreaseCurrentLives();

	if (PlayerStateRef->GetCurrentLives() <= 0)
	{
		bIsLastLife = true;
	}

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->UpdateLives(PlayerStateRef->GetCurrentLives());
	}
}

void AECPlayerController::MoveToSide(float Value)
{
	if (!bCanMove || bLockMovement || !IsValid(PlayerRef) || !IsValid(PlatformsContainer) || !IsValid(CurrentPlatform)) return;

	if (FMath::IsNearlyZero(Value)) return;

	if ((Value < 0.0f && bIsMovingLeft) || (Value > 0.0f && bIsMovingRight)) return;

	int LanesCount = CurrentPlatform->GetLanesArray().Num();

	bLockMovement = true;
	CurrentLockMovementTime = 0.0f;

	PrintDebugLog(FString::Printf(TEXT("LanesCount: %d"), LanesCount));
	PrintDebugLog(FString::Printf(TEXT("Current Lane: %d"), CurrentLane));

	// Get Rotation Rate
	float RotationRate = GetCurrentRotationRate();

	// Move Left
	if (Value < 0.0f)
	{
		if (CurrentLane - 1 >= 0)
			CurrentLane--;
		else
			CurrentLane = LanesCount - 1;

		bIsMovingRight = false;
		bIsMovingLeft = true;
		PlatformsContainer->RotateRight(RotationRate);
		PlayerRef->MoveLeft();
	}
	// Move Right
	else
	{
		if (CurrentLane + 1 < LanesCount)
			CurrentLane++;
		else
			CurrentLane = 0;

		bIsMovingLeft = false;
		bIsMovingRight = true;
		PlatformsContainer->RotateLeft(RotationRate);
		PlayerRef->MoveRight();
	}

	PrintDebugLog(FString::Printf(TEXT("Next Lane: %d"), CurrentLane));
}

float AECPlayerController::GetCurrentRotationRate()
{
	float RotationRate = 0;

	if (!IsValid(PlayerRef) || !IsValid(CurrentPlatform) || !(bIsMovingLeft || bIsMovingRight))
	{
		return RotationRate;
	}

	float PlayerVelocity = PlayerRef->GetVelocity().X;
	float PlatformRadius = CurrentPlatform->GetPlatformRadius() / 100; // Convert to meters

	/** To get the rotation rate (radial velocity), which is equivilant to the linear velocity / radius
	* However, to get the velocity, we need to extract the velocity on the Axis which the rotation happens on, which in this case is the Y axis
	* And to get the linear velocity on the Y axis, we can use the Cosine of the velocity's lean angle
	* the lean angle is the complementary angle of the player's rotation when moving to the side
	* then we can extract the linear velocity on the Y axis as = Player Velocity * Cos(Velocity Lean angle)
	* we divide all that by the platform radius, and we get the radial velocity at which the platforms should rotate
	*/
	RotationRate = PlayerVelocity * FMath::Cos(90 - PlayerRef->GetSideMoveRotation()) / PlatformRadius;

	return RotationRate;
}

void AECPlayerController::Jump()
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
		AECCharacter* PlayerTemp = PlayerRef;
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

void AECPlayerController::Slide()
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

void AECPlayerController::Respawn()
{
	CurrentLane = 1; // Set to middle
	bIsRunning = false;
	bIsMovingLeft = bIsMovingRight = false;
	PlayerRef->Respawn();

	bIsRunning = false;
}

void AECPlayerController::OnCollectCoin()
{
	PlayerStateRef->IncreaseCoinsCollected();

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->UpdateCoins(PlayerStateRef->GetCoinsCollected());
	}
}

void AECPlayerController::OnKillEnemy(AEnemy* KilledEnemy)
{
	PrintDebugLog("Enemy Killed");

	PlayerStateRef->IncreaseEnemiesKilled();

	AECGameMode_Level* GameMode = Cast<AECGameMode_Level>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnEnemyKilled(KilledEnemy);

	int Level = 1;
	AECGameState* GameState = GetWorld()->GetGameState<AECGameState>();
	if (GameState)	Level = GameState->GetCurrentLevel();

	if (IsValid(InGameUIWidgetInstance))
	{
		InGameUIWidgetInstance->UpdateLevel(Level);
	}
}

void AECPlayerController::AddToCurrentDistance(float Distance)
{
	if (bIsRunning)
	{
		PlayerStateRef->AddDistance(Distance);

		if (IsValid(InGameUIWidgetInstance))
		{
			InGameUIWidgetInstance->UpdateDistance(PlayerStateRef->GetCurrentDistance());
		}
	}
}