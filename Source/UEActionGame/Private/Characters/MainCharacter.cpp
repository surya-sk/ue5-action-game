// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(GetMesh(), TEXT("headSocket"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainCharacter::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f); // only need the yaw

		const FVector DirectionVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(DirectionVector, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f); // only need the yaw

		const FVector DirectionVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(DirectionVector, Value);
	}
}

void AMainCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMainCharacter::Vault()
{
	bool bShouldClimb;
	bool bWallThick = true;
	const FVector Start = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 44.f);
	const FVector End = (GetActorForwardVector() * 70.f) + Start;
	const TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	FHitResult BreakHitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(NULL);
	bool bLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, TraceObjects, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
		BreakHitResult, true);
	if (bLineTrace)
	{
		FVector WallLocation = BreakHitResult.Location;
		FVector Normal = BreakHitResult.Normal;
		auto Rot = UKismetMathLibrary::MakeRotFromX(Normal);
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Rot);
		const FVector VStart = (ForwardVector * (-10)) + WallLocation + FVector(0, 0, 200.f);
		const FVector VEnd = Start - FVector(0, 0, 200.f);
		FHitResult HitResult;
		bool bVLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(this, VStart, VEnd, TraceObjects, false, ActorsToIgnore,
			EDrawDebugTrace::ForDuration, HitResult, true);
		if (bVLineTrace)
		{
			FVector WallHeight = HitResult.Location;
			FVector Diff = WallHeight - WallLocation;
			if (Diff.Z > 60.f)
			{
				bShouldClimb = true;
				auto CRot = UKismetMathLibrary::MakeRotFromX(Normal);
				FVector CForwardVector = UKismetMathLibrary::GetForwardVector(CRot);
				const FVector CStart = (CForwardVector * (-50.f)) + WallLocation + FVector(0, 0, 250.f);
				const FVector CEnd = CStart - FVector(0, 0, 300.f);
				FHitResult CHitResult;
				bool CLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(this, CStart, CEnd, TraceObjects, false, ActorsToIgnore,
					EDrawDebugTrace::ForDuration, CHitResult, true);
				if (CLineTrace)
				{
					FVector CWallHeight = CHitResult.Location;
					FVector CDiff = WallHeight - CWallHeight;
					bWallThick = CDiff.Z > 30.f ? false : true;
				}
				else
				{
					bWallThick = false;
				}
			}

		}
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AMainCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
}

