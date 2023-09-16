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
#include "Items/Torches/FireTorch.h"
#include "Items/ExpositionNote.h"
#include "Components/AttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Enemy.h"
#include "Components/WidgetComponent.h"
#include "HUD/MainHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Progression/Quest.h"
#include "Characters/NPC.h"

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

	BladeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BladeMesh"));
	BladeMesh->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
}


void AMainCharacter::SetDialogueState(bool bDialogue, ANPC* ActiveNPC)
{
	NPC = ActiveNPC;
	if (bDialogue)
	{
		CharacterActionState = ECharacterActionState::ECAS_Dialogue;
	}
	else
	{
		CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
	}
}

void AMainCharacter::SetDialogueText(FText Dialogue)
{
	if (Overlay)
	{
		Overlay->SetDialogueText(Dialogue);
	}
}

void AMainCharacter::PlayDialogueAudio(USoundBase* DialogueAudio)
{
	if (DialogueAudio)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DialogueAudio, GetActorLocation());
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitPlayerOverlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	Tags.Add(FName("PlayerCharacter"));
}

void AMainCharacter::InitPlayerOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
		if (MainHUD)
		{
			Overlay = MainHUD->GetPlayerOverlay();
			if (Overlay && Attributes)
			{
				Overlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				Overlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
				if (Quest == nullptr)
				{
					TArray<AActor*> ActorsToFind;
					if (UWorld* World = GetWorld())
					{
						UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuest::StaticClass(), ActorsToFind);
						if (ActorsToFind.Num() > 0)
						{
							AQuest* QuestToFind = Cast<AQuest>(ActorsToFind[0]);
							if (QuestToFind)
							{
								Quest = QuestToFind;
							}
						}
					}
				}
				if (Quest)
				{
					Quest->OnObjectiveUpdated.AddDynamic(this, &AMainCharacter::ObjectiveActivated);
					Overlay->SetObjectiveText(Quest->GetCurrentObjective());
				}
			}
		}
	}
}

void AMainCharacter::MoveForward(float Value)
{
	if (CharacterActionState > ECharacterActionState::ECAS_Crouching) return;

	if (CharacterActionState == ECharacterActionState::ECAS_Swimming && Value != 0.f)
	{
		const FVector Direction = ViewCamera->GetForwardVector();
		AddMovementInput(Direction, Value);
	}
	else if(Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f); // only need the yaw

		const FVector DirectionVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(DirectionVector, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (CharacterActionState > ECharacterActionState::ECAS_Crouching) return;;
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
	if (CharacterActionState > ECharacterActionState::ECAS_Crouching) return;
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
	if (CharacterActionState == ECharacterActionState::ECAS_Swimming) return;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCapsuleComponent()->SetCapsuleHalfHeight(30.f);

	float MontageLen = this->PlayAnimMontage(SlideMontage);
	FTimerDelegate Delegate;
	Delegate.BindLambda([&]()
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
		}
	);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, Delegate, MontageLen, false);
}

void AMainCharacter::InteractKeyPressed()
{
	if (NPC)
	{
		NPC->Interact();
	}

	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		EquippedWeapon = OverlappingWeapon;
		EquippedWeapon->Equip(this->GetMesh(), FName("LeftHandSocket"), this, this);
		CharacterWeaponState = ECharacterWeaponState::ECWS_Equipped;
		OverlappingItem = nullptr;
		return;
	}
	
	AFireTorch* OverlappingTorch = Cast<AFireTorch>(OverlappingItem);
	if (OverlappingTorch)
	{
		if (EquippedWeapon)
		{
			Equip(); // actually unequips
		}
		EquippedTorch = OverlappingTorch;
		EquippedTorch->Equip(this->GetMesh(), FName("RightHandSocket"), this, this);
		this->Tags.Add(FName("Torch"));
		CharacterWeaponState = ECharacterWeaponState::ECWS_Torch;
		OverlappingItem = nullptr;
		return;
	}

	AExpositionNote* Note = Cast<AExpositionNote>(OverlappingItem);
	if (Note)
	{
		OverlappingNote = Note;
		OverlappingNote->Equip(this->GetMesh(), FName("RightHandSocket"), this, this);
		OverlappingItem = nullptr;
		return;
	}
	
}

void AMainCharacter::HitReactEnd()
{
	CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
}

void AMainCharacter::EndDodge()
{
	CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
	CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
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
		CharacterWeaponState == ECharacterWeaponState::ECWS_Equipped;
	bool bCanEquip = CharacterActionState == ECharacterActionState::ECAS_Unoccupied &&
		CharacterWeaponState != ECharacterWeaponState::ECWS_Equipped && EquippedWeapon;
	if (bCanUnequip)
	{
		PlayEquipMontage(FName("Unequip"));
		CharacterWeaponState = ECharacterWeaponState::ECWS_Unequipped;
		CharacterActionState = ECharacterActionState::ECAS_EquippingWeapon;
	}
	else if (bCanEquip)
	{
		UnequipTorch();
		PlayEquipMontage(FName("Equip"));
		CharacterWeaponState = ECharacterWeaponState::ECWS_Equipped;
		CharacterActionState = ECharacterActionState::ECAS_EquippingWeapon;
	}
}

void AMainCharacter::Sprint()
{
	if (CanSprint())
	{
		bSprinting = true;
		CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		bSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
}

void AMainCharacter::StopSprinting()
{
	bSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMainCharacter::StartCrouch()
{
	if (GetCharacterMovement()->MaxWalkSpeed > WalkSpeed) return;
	Super::Crouch();
	CharacterActionState = ECharacterActionState::ECAS_Crouching;
}

void AMainCharacter::StopCrouching()
{
	Super::UnCrouch();
	CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
}

void AMainCharacter::PerformTakedown()
{
	if (EnemyToAssassinate)
	{
		BladeMesh->SetHiddenInGame(false);
		PlayTakedownMontage();
		EnemyToAssassinate->PlayAssassinationMontage();
	}
}

void AMainCharacter::UnequipTorch()
{
	if (EquippedTorch)
	{
		EquippedTorch->Unequip();
		this->Tags.Remove(FName("Torch"));
		CharacterWeaponState = ECharacterWeaponState::ECWS_Unequipped;
		EquippedTorch = nullptr;
	}

	if (OverlappingNote)
	{
		OverlappingNote->Unequip();
		OverlappingNote = nullptr;
	}
}

void AMainCharacter::Dodge()
{
	if (CharacterActionState != ECharacterActionState::ECAS_Unoccupied) return;
	PlayDodgeMontage();
	CharacterActionState = ECharacterActionState::ECAS_Dodge;
}

void AMainCharacter::HideBlade()
{
	BladeMesh->SetHiddenInGame(true);
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

void AMainCharacter::PlayTakedownMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && TakedownMontage)
	{
		AnimInstance->Montage_Play(TakedownMontage);
	}
}

void AMainCharacter::PlayDodgeMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DodgeMontage)
	{
		AnimInstance->Montage_Play(DodgeMontage);
	}
}


void AMainCharacter::AttackEnd()
{
	CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
}

void AMainCharacter::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Hitter);

	SetWeaponCollision(ECollisionEnabled::NoCollision);
	if(Attributes && !Attributes->IsDead())
		CharacterActionState = ECharacterActionState::ECAS_HitReaction;
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
	return CharacterActionState == ECharacterActionState::ECAS_Unoccupied &&
		CharacterWeaponState == ECharacterWeaponState::ECWS_Equipped ;
}

void AMainCharacter::Die()
{
	Super::Die();
	CharacterActionState = ECharacterActionState::ECAS_Dead;
	DisableMeshCollision();
}

FVector AMainCharacter::GetEnemyWarpTarget()
{
	if(EnemyToAssassinate == nullptr) return FVector();
	FVector TargetToAttacker = (GetActorLocation() - EnemyToAssassinate->GetActorLocation()).GetSafeNormal();
	return TargetToAttacker + EnemyToAssassinate->GetActorLocation();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Attributes->RegenrateHealth();
	if (!Attributes->HasEnoughStamina())
	{
		StopSprinting();
	}
	Attributes->HandleStamina(bSprinting);
	if (Overlay)
	{
		Overlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}

	if (CharacterActionState != ECharacterActionState::ECAS_Swimming && GetCharacterMovement()->IsSwimming())
	{
		CharacterActionState = ECharacterActionState::ECAS_Swimming;
		if (WaterSound)
		{
			WaterAudio = UGameplayStatics::SpawnSound2D(GetWorld(), WaterSound);
		}
		UnequipTorch();
		Equip();
	}

	if (CharacterActionState == ECharacterActionState::ECAS_Swimming && !GetCharacterMovement()->IsSwimming())
	{
		CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
		if (WaterAudio)
		{
			WaterAudio->SetActive(false);
		}
		Attributes->RegenrateOxygen();
	}

	Attributes->HandleOxygen(CharacterActionState == ECharacterActionState::ECAS_Swimming);
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
	PlayerInputComponent->BindAction(FName("Unequip"), IE_Pressed, this, &AMainCharacter::UnequipTorch);
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Pressed, this, &AMainCharacter::Sprint);
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Released, this, &AMainCharacter::StopSprinting);
	PlayerInputComponent->BindAction(FName("Crouch"), IE_Pressed, this, &AMainCharacter::StartCrouch);
	PlayerInputComponent->BindAction(FName("Crouch"), IE_Released, this, &AMainCharacter::StopCrouching);
	PlayerInputComponent->BindAction(FName("Takedown"), IE_Pressed, this, &AMainCharacter::PerformTakedown);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &AMainCharacter::Dodge);
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	if (Overlay)
	{
		Overlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
	return DamageAmount;
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
}

bool AMainCharacter::CanSprint()
{
	if (CharacterActionState == ECharacterActionState::ECAS_Swimming || CharacterActionState == ECharacterActionState::ECAS_Following) return false;
	return CharacterActionState <= ECharacterActionState::ECAS_Crouching &&
		CharacterWeaponState == ECharacterWeaponState::ECWS_Unequipped && Attributes->HasEnoughStamina();
}

void AMainCharacter::ObjectiveActivated()
{
	if (Overlay)
	{
		Overlay->SetObjectiveText(Quest->GetCurrentObjective());
	}
}

