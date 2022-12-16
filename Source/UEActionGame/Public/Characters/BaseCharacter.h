// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class UEACTIONGAME_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/** COMBAT*/

	/// <summary>
	/// Moves towards the combat target and plays the attack montage
	/// </summary>
	virtual void Attack();

	/// <summary>
	/// Plays the appropriate HitReact sections based on the direction of hit
	/// </summary>
	/// <param name="ImpactPoint">The point where the actor is hit</param>
	void DirectionalHitReact(const FVector& ImpactPoint);

	/// <summary>
	/// Handles hit reaction
	/// </summary>
	/// <param name="ImpactPoint">Point of impact</param>
	virtual void GetHit(const FVector& ImpactPoint) override;

	/// <summary>
	/// Determines if the character can attack
	/// </summary>
	/// <returns>If the character is all set to attack</returns>
	virtual bool CanAttack();

	/// <summary>
	/// Checks if the character is not dead
	/// </summary>
	/// <returns>If the character is alive</returns>
	bool IsAlive();
	
	/// <summary>
	/// Kills the character
	/// </summary>
	virtual void Die();

	/// <summary>
	/// Callback for attack montage notify to end attack state
	/// </summary>
	UFUNCTION(BlueprintCallable)
	virtual	void AttackEnd();

	/// <summary>
	/// Disables the character's capsule compoenent
	/// </summary>
	void DisableCapsule();

	/// <summary>
	/// Handles damage that has been inflicted
	/// </summary>
	/// <param name="DamageAmount">The amount of damage dealt</param>
	virtual void HandleDamage(float DamageAmount);

	/// <summary>
	/// Plays the hit impact sound
	/// </summary>
	/// <param name="ImpactPoint">the point of impact</param>
	void PlayHitSound(const FVector& ImpactPoint);

	/// <summary>
	/// Spawns impact particles
	/// </summary>
	/// <param name="ImpactPoint"></param>
	void SpawnHitParticles(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled);


	/** MONTAGE FUNCTIONS*/

	/// <summary>
	/// Plays the HitReact montage and jumps to the specified section
	/// </summary>
	/// <param name="SectionName">The section to jump to</param>
	void PlayHitReactMontage(const FName SectionName);

	/// <summary>
	/// Plays the attack montage
	/// </summary>
	/// <returns>The index of the random section</returns>
	virtual int32 PlayAttackMontage();

	/// <summary>
	/// Plays the death montage
	/// </summary>
	/// <returns>The index of the random sections</returns>
	virtual int32 PlayDeathMontage();


	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

private:
	/// <summary>
	/// Plays the specified montage and jumps to the specified section
	/// </summary>
	/// <param name="Montage">The montage to play</param>
	/// <param name="SectionName">The section to jump to</param>
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	/// <summary>
	/// Plays the montage and jumps to a random section
	/// </summary>
	/// <param name="Montage">The montage to play</param>
	/// <param name="Sections">An array of sections</param>
	/// <returns>The index of the random section</returns>
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& Sections);

	UPROPERTY(EditAnywhere, Category = Effects)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Effects)
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

};
