// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PresentDayCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Progression/SaveSystem.h"
#include "HUD/MainHUD.h"
#include "Progression/Quest.h"
#include "HUD/PlayerOverlay.h"

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

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(GetMesh(), TEXT("HeadSocket"));

	TorchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorchMesh"));
	TorchMesh->SetupAttachment(GetMesh(), TEXT("HeadSocket"));

	TorchLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("TorchLight"));
	TorchLight->SetupAttachment(TorchMesh);
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
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &APresentDayCharacter::ToggleFlashlight);
}

void APresentDayCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	LoadGame();
	InitObjectiveText();
	InitPauseOverlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	Tags.Add(FName("PlayerCharacter"));
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
void APresentDayCharacter::ToggleFlashlight()
{
	TorchLight->ToggleVisibility();
	if(FlashlightSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FlashlightSound, GetActorLocation());
	}
}

void APresentDayCharacter::InitObjectiveText()
{
	if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
		if (MainHUD)
		{
			Overlay = MainHUD->GetPlayerOverlay();
			if (Overlay)
			{
				if (QuestRef == nullptr)
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
								QuestRef= QuestToFind;
							}
						}
					}
				}
				if (QuestRef)
				{
					QuestRef->OnObjectiveUpdated.AddDynamic(this, &APresentDayCharacter::OnObjectiveActivated);
					Overlay->SetObjectiveText(QuestRef->GetCurrentObjective());
				}
			}
		}
	}
}

void APresentDayCharacter::OnObjectiveActivated()
{
	if (Overlay)
	{
		Overlay->SetObjectiveText(QuestRef->GetCurrentObjective());
	}
}

void APresentDayCharacter::SaveGame(bool bSavePosition) const
{
	auto* SaveSystem = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(USaveSystem::StaticClass()));

	SaveSystem->PlayerData.bWeaponEquipped = false;
	SaveSystem->PlayerData.LastMapName = GetWorld()->GetMapName();
	SaveSystem->PlayerData.Location = GetActorLocation();
	SaveSystem->PlayerData.Rotation = GetActorRotation();
	SaveSystem->PlayerData.PresentCurrentObjectiveIndex = CurrentObjectiveIndex;
	SaveSystem->PlayerData.bLoadPosition = bSavePosition;

	UGameplayStatics::SaveGameToSlot(SaveSystem, SaveSystem->PlayerName, SaveSystem->UserIndex);
}

void APresentDayCharacter::LoadGame()
{
	if (auto* SaveSystem = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(USaveGame::StaticClass())))
	{
		SaveSystem = Cast<USaveSystem>(UGameplayStatics::LoadGameFromSlot(SaveSystem->PlayerName, SaveSystem->UserIndex));

		SetActorLocation(SaveSystem->PlayerData.Location);
		SetActorRotation(SaveSystem->PlayerData.Rotation);
		InitObjectiveText();
	}
}
