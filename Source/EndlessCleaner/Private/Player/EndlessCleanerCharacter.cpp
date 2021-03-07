// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EndlessCleanerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEndlessCleanerCharacter::AEndlessCleanerCharacter()
{
	// Setup Initial variable values
	CharacterMaxSpeed = 600.f;
	SideMoveRotation = 45.0f;

	// Setup Capsule Size
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Setup Camera
	// Don't rotate character
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Create Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);	// To not rotate the camera
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;	// Disable the collision detection of Camera

	// Create Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Seperate Camera and Boom rotation
	CameraComponent->SetRelativeRotation(FRotator(10.f, 0.f, 0.f));

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// To make sure this is set before it is modified by the player controller
	bIsMoving = false;
}

// Called when the game starts or when spawned
void AEndlessCleanerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set Max Speed
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxSpeed;
}

// Called every frame
void AEndlessCleanerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward();
}

// Called to bind functionality to input
void AEndlessCleanerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEndlessCleanerCharacter::MoveForward() {
	if (!bIsMoving)	return;

	const FRotator Rotation = GetActorRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Move the character
	AddMovementInput(Direction, 1.0f);
}

void AEndlessCleanerCharacter::MoveLeft() {
	if (!bIsMoving)	return;

	UE_LOG(LogTemp, Warning, TEXT("Move To Left."));

	SetActorRelativeRotation(FRotator(0, -SideMoveRotation, 0));
}

void AEndlessCleanerCharacter::MoveRight() {
	if (!bIsMoving)	return;

	UE_LOG(LogTemp, Warning, TEXT("Move To Right."));

	SetActorRelativeRotation(FRotator(0, SideMoveRotation, 0));
}

void AEndlessCleanerCharacter::StopMoveToSide() {
	if (!bIsMoving)	return;

	UE_LOG(LogTemp, Warning, TEXT("Stop Move To Side."));

	SetActorRelativeRotation(FRotator::ZeroRotator);
}

void AEndlessCleanerCharacter::Respawn(FVector Position) {
	SetActorLocation(Position);
	StopMoveToSide();

	bIsMoving = false;
}