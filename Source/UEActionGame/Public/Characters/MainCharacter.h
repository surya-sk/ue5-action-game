// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class UEACTIONGAME_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterWeaponState GetCharacterWeaponState() const { return CharacterWeaponState; }

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

	/** INPUT ACTIONS*/

	/// <summary>
	/// Vaults over the wall if possible
	/// </summary>
	void Vault();

	/// <summary>
	/// Slides under the object if appropriate 
	/// </summary>
	void Slide();

	/// <summary>
	/// Interacts with object
	/// </summary>
	void InteractKeyPressed();

	/// <summary>
	/// Equip weapon
	/// </summary>
	void Equip();

	/// <summary>
	/// Start sprinting
	/// </summary>
	void Sprint();

	/// <summary>
	/// Sets the speed back to walk speed
	/// </summary>
	void StopSprinting();


	/** COMBAT*/

	/// <summary>
	/// Plays the weapon equip montage
	/// </summary>
	/// <param name="Section">The montage section to jump to</param>
	void PlayEquipMontage(const FName Section);

	/// <summary>
	/// Unequips the weapon
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	/// <summary>
	/// Arms the weapon
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void AttackWeaponToHand();

	/// <summary>
	/// Callback for anim notify
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void FinishEquip();

	/// <summary>
	/// Callback for anim notify
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	/** <ABaseCharacter> */
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual bool CanAttack() override;
	/** </ABaseCharacter> */

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 50.f;

	UPROPERTY(EditAnywhere)
	float JogSpeed = 100.f;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 300.f;

private:

	/// <summary>
	/// Resets collision presets and movement values to default
	/// </summary>
	void ResetCollisionAndMovement();

	/// <summary>
	/// Sets collision for vaulting
	/// </summary>
	void SetVaultingCollision();

	/// <summary>
	/// Vaults or climbs depending on various parameters
	/// </summary>
	/// <param name="bShouldClimb">Determines whether to climb</param>
	/// <param name="bWallThick">If the wall is too thick to climb</param>
	/// <param name="bCanClimb">If the wall is climbable</param>
	/// <param name="ForwardVector">The forward vector of the wall</param>
	/// <param name="CWallHeight">The wall height</param>
	void VaultOrClimb(bool bShouldClimb, bool bWallThick, bool bCanClimb, FVector ForwardVector, FVector CWallHeight);

	/// <summary>
	/// Checks if the player can sprint
	/// </summary>
	/// <returns></returns>
	bool CanSprint();

	bool bIsClimbing = false;

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

	/** MONTAGES*/

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
	UAnimMontage* EquipMontage;
};
