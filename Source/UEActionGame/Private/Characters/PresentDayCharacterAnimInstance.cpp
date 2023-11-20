// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PresentDayCharacterAnimInstance.h"
#include "Characters/PresentDayCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPresentDayCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APresentDayCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		CharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UPresentDayCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
	}
}
