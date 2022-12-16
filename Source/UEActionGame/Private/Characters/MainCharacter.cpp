// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	
	Tags.Add(FName("PlayerCharacter"));
}

void AMainCharacter::MoveForward(float Value)
{
	if (CharacterActionState != ECharacterActionState::ECAS_Unoccupied) return;
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
	if (CharacterActionState != ECharacterActionState::ECAS_Unoccupied) return;
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
	if (CharacterActionState != ECharacterActionState::ECAS_Unoccupied) return;
	bool bShouldClimb;
	bool bWallThick;
	bool bCanClimb = true;
	FVector CWallHeight = FVector(0, 0, 0);
	if (bIsClimbing)
		return;
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
				CWallHeight = CHitResult.Location;
				FVector CDiff = WallHeight - CWallHeight;
				bWallThick = CDiff.Z < 30.f;
				
				VaultOrClimb(bShouldClimb, bWallThick, bCanClimb, ForwardVector, CWallHeight);
			}
			else
			{
				bWallThick = false;
				VaultOrClimb(bShouldClimb, bWallThick, bCanClimb, ForwardVector, CWallHeight);
			}
		}
	}
}

void AMainCharacter::Slide()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	float MontageLen = this->PlayAnimMontage(SlideMontage);
	FTimerDelegate Delegate;
	Delegate.BindLambda([&]()
		{
			ResetCollisionAndMovement();
		}
	);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, MontageLen, false);
}

void AMainCharacter::InteractKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		EquippedWeapon = OverlappingWeapon;
		EquippedWeapon->Equip(this->GetMesh(), FName("LeftHandSocket"), this, this);
		CharacterWeaponState = ECharacterWeaponState::ECWS_Equipped;
		OverlappingItem = nullptr;
	}
}

void AMainCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		CharacterActionState = ECharacterActionState::ECAS_Attacking;
	}
}

void AMainCharacter::Equip()
{
	bool bCanUnequip = CharacterActionState == ECharacterActionState::ECAS_Unoccupied &&
		CharacterWeaponState != ECharacterWeaponState::ECWS_Unequipped;
	bool bCanEquip = CharacterActionState == ECharacterActionState::ECAS_Unoccupied &&
		CharacterWeaponState == ECharacterWeaponState::ECWS_Unequipped && EquippedWeapon;
	if (bCanUnequip)
	{
		PlayEquipMontage(FName("Unequip"));
		CharacterWeaponState = ECharacterWeaponState::ECWS_Unequipped;
		CharacterActionState = ECharacterActionState::ECAS_EquippingWeapon;
	}
	else if (bCanEquip)
	{
		PlayEquipMontage(FName("Equip"));
		CharacterWeaponState = ECharacterWeaponState::ECWS_Equipped;
		CharacterActionState = ECharacterActionState::ECAS_EquippingWeapon;
	}
}

void AMainCharacter::PlayEquipMontage(const FName Section)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(Section, EquipMontage);
	}
}


void AMainCharacter::AttackEnd()
{
	CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
}

void AMainCharacter::GetHit(const FVector& ImpactPoint)
{
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void AMainCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(this->GetMesh(), FName("SpineSocket"));
	}
}

void AMainCharacter::AttackWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(this->GetMesh(), FName("LeftHandSocket"));
	}
}

void AMainCharacter::FinishEquip()
{
	CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
}

bool AMainCharacter::CanAttack()
{
	return CharacterActionState == ECharacterActionState::ECAS_Unoccupied && CharacterWeaponState != ECharacterWeaponState::ECWS_Unequipped;
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
	PlayerInputComponent->BindAction(FName("Slide"), IE_Pressed, this, &AMainCharacter::Slide);
	PlayerInputComponent->BindAction(FName("Interact"), IE_Pressed, this, &AMainCharacter::InteractKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AMainCharacter::Equip);
}

void AMainCharacter::ResetCollisionAndMovement()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AMainCharacter::SetVaultingCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void AMainCharacter::VaultOrClimb(bool bShouldClimb, bool bWallThick, bool bCanClimb, FVector ForwardVector, FVector CWallHeight)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects = TArray<TEnumAsByte<EObjectTypeQuery>>();
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(NULL);
	float MontageSeconds;
	if (!bShouldClimb)
	{
		bIsClimbing = true;
		SetVaultingCollision();
		if (!bWallThick)
		{
			MontageSeconds = this->PlayAnimMontage(GettingUp);
		}
		else
		{
			MontageSeconds = this->PlayAnimMontage(VaultMontage);
		}
		FTimerDelegate Delegate;
		Delegate.BindLambda([&]()
			{
				UE_LOG(LogTemp, Warning, TEXT("Delegate!"));
		ResetCollisionAndMovement();
		bIsClimbing = false;
			}
		);
		FTimerHandle handle;

		GetWorld()->GetTimerManager().SetTimer(handle, Delegate, MontageSeconds - 0.5f, false);
	}
	else
	{
		FHitResult CheckHitResult;
		const FVector CheckStart = GetActorLocation() + FVector(0, 0, 200.f);
		const FVector CheckEnd = (GetActorForwardVector() * 70.f) + CheckStart;
		bool CheckLineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(this, CheckStart, CheckEnd, TraceObjects, false, ActorsToIgnore,
			EDrawDebugTrace::ForDuration, CheckHitResult, true);
		if (CheckLineTrace)
		{
			bCanClimb = false;
		}
		const FVector CheckStart2 = GetActorLocation();
		const FVector CheckEnd2 = CheckStart2 + FVector(0, 0, 200.f);
		bool bCheckLineTrace2 = UKismetSystemLibrary::LineTraceSingleForObjects(this, CheckStart2, CheckEnd2, TraceObjects, false, ActorsToIgnore,
			EDrawDebugTrace::ForDuration, CheckHitResult, true);

		if (bCanClimb)
		{
			bIsClimbing = true;
			SetVaultingCollision();
			SetActorLocation((ForwardVector * 50.f) + GetActorLocation());
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, CWallHeight.Z - 44.f));
			MontageSeconds = this->PlayAnimMontage(ClimbUpMontage);
			FTimerDelegate Delegate;
			Delegate.BindLambda([&]()
				{
					if (bWallThick)
					{
						ResetCollisionAndMovement();
					}
					else
					{
						if (bIsClimbing) return;
						auto MontageLen = this->PlayAnimMontage(JumpDownMontage);
						FTimerDelegate Del;
						Del.BindLambda([&]()
							{
								ResetCollisionAndMovement();
							});
						FTimerHandle h;
						GetWorld()->GetTimerManager().SetTimer(h, Del, MontageLen, false);
						bIsClimbing = false;
					}
				}
			);
			FTimerHandle handle;

			GetWorld()->GetTimerManager().SetTimer(handle, Delegate, MontageSeconds, false);
			bIsClimbing = false;
		}
	}
}

