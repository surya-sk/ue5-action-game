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
}

void APresentDayCharacter::BeginPlay()
{
	Super::BeginPlay();
}
