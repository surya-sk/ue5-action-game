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
class AEnemy;
class AFireTorch;

UCLASS()
class UEACTIONGAME_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterWeaponState GetCharacterWeaponState() const { return CharacterWeaponState; }
	FORCEINLINE ECharacterActionState GetCharacterActionState() const { return CharacterActionState; }
	FORCEINLINE void SetEnemyToAssassinate(AEnemy* Enemy) { EnemyToAssassinate = Enemy; }
	FORCEINLINE void SetFollowState(bool bFollow) 
	{ 
		if (bFollow)
			CharacterActionState = ECharacterActionState::ECAS_Following;
		else
			CharacterActionState = ECharacterActionState::ECAS_Unoccupied;
	}
	FORCEINLINE bool IsFollowing() { return CharacterActionState == ECharacterActionState::ECAS_Following; }
	void SetDialogueState(bool bDialogue, class ANPC* ActiveNPC = nullptr);
	void SetDialogueText(FText Dialogue);
	void PlayDialogueAudio(USoundBase* DialogueAudio);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitPlayerOverlay();

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

	/// <summary>
	/// Sets the player action to crouch
	/// </summary>
	void StartCrouch();

	/// <summary>
	/// Stops the player from crouching
	/// </summary>
	void StopCrouching();

	/// <summary>
	/// Takedown if an enemy is present
	/// </summary>
	void PerformTakedown();

	/// <summary>
	/// Unequips the torch
	/// </summary>
	void UnequipItem();

	/// <summary>
	/// Performs a dodge
	/// </summary>
	void Dodge();

	/// <summary>
	/// Hides the blade
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void HideBlade();


	/** COMBAT*/

	/// <summary>
	/// Plays the weapon equip montage
	/// </summary>
	/// <param name="Section">The montage section to jump to</param>
	void PlayEquipMontage(const FName Section);

	/// <summary>
	/// Plays the stealth takedown montage
	/// </summary>
	void PlayTakedownMontage();

	/// <summary>
	/// Plays the dodge montage
	/// </summary>
	void PlayDodgeMontage();

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

	/// <summary>
	/// Gets the character out of the dodge state
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void EndDodge();

	/** <ABaseCharacter> */
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual bool CanAttack() override;
	virtual void Die() override;
	/** </ABaseCharacter> */

	UFUNCTION(BlueprintCallable)
	FVector GetEnemyWarpTarget();

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 50.f;

	UPROPERTY(EditAnywhere)
	float JogSpeed = 100.f;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 300.f;

	UPROPERTY(BlueprintReadOnly)
	AEnemy* EnemyToAssassinate;

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

	UFUNCTION()
	void ObjectiveActivated();

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

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BladeMesh;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleInstanceOnly)
	AFireTorch* EquippedTorch;

	UPROPERTY()
	class AExpositionNote* OverlappingNote;

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

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* TakedownMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere)
	class USoundBase* WaterSound;

	bool bSprinting;

	UAudioComponent* WaterAudio;

	UPROPERTY()
	class UPlayerOverlay* Overlay;

	UPROPERTY(EditInstanceOnly)
	class AQuest* Quest;

	class ANPC* NPC;
};
