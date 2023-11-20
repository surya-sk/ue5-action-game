// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PresentDayCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APresentDayCharacter::APresentDayCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	CamBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CamBoom->SetupAttachment(GetRootComponent());
	CamBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CamBoom);
}

void APresentDayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APresentDayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &APresentDayCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &APresentDayCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &APresentDayCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &APresentDayCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Pressed, this, &APresentDayCharacter::StartSprinting);
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Released, this, &APresentDayCharacter::StopSprinting);
}

void APresentDayCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APresentDayCharacter::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f); // only need the yaw

		const FVector DirectionVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(DirectionVector, Value);
	}
}

void APresentDayCharacter::MoveRight(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f); // only need the yaw

		const FVector DirectionVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(DirectionVector, Value);
	}
}

void APresentDayCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APresentDayCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APresentDayCharacter::StartSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

void APresentDayCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
