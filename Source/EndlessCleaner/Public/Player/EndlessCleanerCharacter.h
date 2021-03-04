// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EndlessCleanerCharacter.generated.h"

UCLASS()
class ENDLESSCLEANER_API AEndlessCleanerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEndlessCleanerCharacter();

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
	void MoveToSide(int Direction);

	void MoveForward();

	FORCEINLINE void SetIsMoving(bool Value) { this->bIsMoving = Value; };

	void Respawn(FVector Position);

private:
	bool bIsMoving = false;

	FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = Character)
		float CharacterMaxSpeed;

	UPROPERTY(EditAnywhere, Category = Character)
		float SideMoveDistance;
};
