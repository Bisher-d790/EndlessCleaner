// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ECCharacter.generated.h"

UCLASS()
class ENDLESSCLEANER_API AECCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AECCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class USpringArmComponent* CameraBoom;

public:
	void MoveLeft();

	void MoveRight();

	void MoveForward();

	void StopMoveToSide();

	FORCEINLINE void SetIsMoving(bool Value) { this->bIsMoving = Value; };

	UFUNCTION(BlueprintCallable, Category = Movement)
		void BlockForwardMovement(bool Value) { this->bIsMovingForwardBlocked = Value; };

	void Respawn();

	FORCEINLINE float GetSideMoveRotation() { return SideMoveRotation; };

private:
	bool bIsMoving = false;

	bool bIsMovingForwardBlocked = false;

	bool bIsRotating = false;

	UPROPERTY(EditAnywhere, Category = Character)
		float SideMoveRotation;

	UPROPERTY(EditAnywhere, Category = Character)
		float SideRotationDuration;

	FRotator TargetRotation;

	FRotator FromRotation;

	float RotationLerpTime = 0.0f;
};
