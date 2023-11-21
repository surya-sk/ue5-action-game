// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PresentDayCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API APresentDayCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APresentDayCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Input axis mapping callbacks
	/// </summary>
	/// <param name="Value">The value given by the input</param>
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void StartSprinting();
	void StopSprinting();

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 200.f;

	UPROPERTY(EditAnywhere)
	float JogSpeed = 500.f;

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CamBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HairMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TorchMesh;

	UPROPERTY(VisibleAnywhere)
	ULightComponent* TorchLight;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY()
	class AExpositionNote* OverlappingNote;
	
};
