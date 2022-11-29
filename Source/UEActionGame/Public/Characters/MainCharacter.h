// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;

UCLASS()
class UEACTIONGAME_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterWeaponState GetCharacterWeaponState() const { return CharacterWeaponState; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void Vault();
	void Slide();
	void InteractKeyPressed();

private:

	ECharacterWeaponState CharacterWeaponState = ECharacterWeaponState::ECWS_Unequipped;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HairMesh;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* GettingUp;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* VaultMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* ClimbUpMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* JumpDownMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* SlideMontage;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	bool bIsClimbing = false;

	void ResetCollisionAndMovement();

	void SetVaultingCollision();

	void VaultOrClimb(bool bShouldClimb, bool bWallThick, bool bCanClimb, FVector ForwardVector, FVector CWallHeight);
};
