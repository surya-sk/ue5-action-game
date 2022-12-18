// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacterAnimInstance.h"
#include "Characters/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if (MainCharacter)
	{
		MainCharacterMovement = MainCharacter->GetCharacterMovement();
	}

}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MainCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MainCharacterMovement->Velocity);
		bIsFalling = MainCharacterMovement->IsFalling();
		CharacterWeaponState = MainCharacter->GetCharacterWeaponState();
		CharacterActionState = MainCharacter->GetCharacterActionState();
		DeathPose = MainCharacter->GetDeathPose();
	}
}
