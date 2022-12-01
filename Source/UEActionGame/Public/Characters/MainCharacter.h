// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class AWeapon;

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

	/// <summary>
	/// Input axis mapping callback
	/// </summary>
	/// <param name="Value"></param>
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	/// <summary>
	/// Input action mapping callbacks
	/// </summary>
	void Vault();
	void Slide();
	void InteractKeyPressed();
	void Attack();
	void Equip();

	/// <summary>
	/// Play montage functions
	/// </summary>
	void PlayAttackMontage();
	void PlayEquipMontage(FName Section);

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void UnarmWeapon();

	UFUNCTION(BlueprintCallable)
	void ArmWeapon();

	UFUNCTION(BlueprintCallable)
	void FinishEquip();

	bool CanAttack();

private:

	ECharacterWeaponState CharacterWeaponState = ECharacterWeaponState::ECWS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterActionState CharacterActionState = ECharacterActionState::ECAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HairMesh;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/// <summary>
	/// Animation montages
	/// </summary>
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

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	bool bIsClimbing = false;

	void ResetCollisionAndMovement();

	void SetVaultingCollision();

	void VaultOrClimb(bool bShouldClimb, bool bWallThick, bool bCanClimb, FVector ForwardVector, FVector CWallHeight);
};
