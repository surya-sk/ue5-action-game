// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Vaulting!!"));
	bool bShouldClimb;
	bool bWallThick = true;
	const FVector Start = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 44.f);
	const FVector End = (GetActorForwardVector() * 70.f) + Start;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects = TArray<TEnumAsByte<EObjectTypeQuery>>();
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	FHitResult BreakHitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(NULL);
	bool bLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, TraceObjects, false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
		BreakHitResult, true);
	if (bLineTrace)
	{
		UE_LOG(LogTemp, Warning, TEXT("First line trace successfull"));
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
			UE_LOG(LogTemp, Warning, TEXT("Second line trace successfull"));
			FVector WallHeight = HitResult.Location;
			FVector Diff = WallHeight - WallLocation;
			bShouldClimb = Diff.Z > 60.f;
			const FVector CStart = (ForwardVector * (-50.f)) + WallLocation + FVector(0, 0, 250.f);
			const FVector CEnd = CStart - FVector(0, 0, 300.f);
			FHitResult CHitResult;
			bool CLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(this, CStart, CEnd, TraceObjects, false, ActorsToIgnore,
				EDrawDebugTrace::ForDuration, CHitResult, true);
			if (CLineTrace)
			{
				UE_LOG(LogTemp, Warning, TEXT("Third line trace"));
				FVector CWallHeight = CHitResult.Location;
				FVector CDiff = WallHeight - CWallHeight;
				bWallThick = CDiff.Z < 30.f;
				float MontageSeconds;
				if (!bShouldClimb)
				{
					UE_LOG(LogTemp, Warning, TEXT("Shouldn't climb"));
					GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
					//SetActorLocation((ForwardVector * 50.f) + GetActorLocation());
					if (!bWallThick)
					{
						MontageSeconds = this->PlayAnimMontage(GettingUp);
					}
					else
					{
						//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, CWallHeight.Z - 0.5f));
						UE_LOG(LogTemp, Warning, TEXT("Play vaulting animation montage"));
						MontageSeconds = this->PlayAnimMontage(VaultMontage);
					}
					FTimerDelegate Delegate;
					Delegate.BindLambda([&]()
						{
							UE_LOG(LogTemp, Warning, TEXT("Delegate!"));
							GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
							GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
							//SetActorRotation(FRotator(GetActorRotation().Roll, GetActorRotation().Pitch, Rot.Yaw + 180.f));
						}
					);
					FTimerHandle handle;

					GetWorld()->GetTimerManager().SetTimer(handle, Delegate, MontageSeconds - 0.2f , false);
				}
			}
			else
			{
				bWallThick = false;
				if (!bShouldClimb)
				{
					float MontageSeconds = 0.f;
					GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
					SetActorLocation((ForwardVector * 50.f) + GetActorLocation());
					if (!bWallThick)
					{
						MontageSeconds =  this->PlayAnimMontage(GettingUp);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Play vaulting animation montage"));
						//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, WallHeight.Z));
						MontageSeconds = this->PlayAnimMontage(VaultMontage);
					}
					FTimerDelegate Delegate;
					Delegate.BindLambda([&]()
						{
							GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					//SetActorRotation(FRotator(GetActorRotation().Roll, GetActorRotation().Pitch, Rot.Yaw + 100.f));
						}
					);
					FTimerHandle handle;

					GetWorld()->GetTimerManager().SetTimer(handle, Delegate, MontageSeconds, false);
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
	PlayerInputComponent->BindAction(FName("Vault"), IE_Pressed, this, &AMainCharacter::Vault);
}

