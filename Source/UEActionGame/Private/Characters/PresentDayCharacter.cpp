// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PresentDayCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

APresentDayCharacter::APresentDayCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
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
