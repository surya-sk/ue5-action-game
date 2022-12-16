// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;

UCLASS()
class UEACTIONGAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	/** <ABaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	virtual void AttackEnd() override;
	/** </ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
	/** AI BEHAVIOUR */

	/// <summary>
	/// Checks if the current patrol target has been reached, and if true chooses a new one
	/// </summary>
	void CheckPatrolTarget();

	/// <summary>
	/// Checks if the combat target in range, and attacks if true
	/// </summary>
	void CheckCombatTarget();

	/// <summary>
	/// Wait time for patrol end has finished, move on to the next one
	/// </summary>
	void PatrolTimerFinished();

	/// <summary>
	/// Starts patrolling to the current patrol target
	/// </summary>
	void StartPatrolling();

	/// <summary>
	/// Chases the combat target
	/// </summary>
	void ChaseTarget();

	/// <summary>
	/// Checks if the target is within the acceptance radius
	/// </summary>
	/// <param name="Target">The target to check, usually the player</param>
	/// <param name="AcceptanceRadius">The radius to check with</param>
	/// <returns>Whether the target is in range</returns>
	bool IsInTargetRange(AActor* Target, double AcceptanceRadius);

	/// <summary>
	/// Moves the enemy to the Target
	/// </summary>
	/// <param name="Target">The actor to move towards</param>
	void MoveToTarget(AActor* Target);

	/// <summary>
	/// Chooses a patrol target at random
	/// </summary>
	/// <returns>The patrol target</returns>
	AActor* ChoosePatrolTarget();

	/// <summary>
	/// Callback for OnPawnSeen for PawnSensing
	/// </summary>
	/// <param name="SeenPawn">The pawn that has been seen</param>
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = AI)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = AI)
	double AttackRadius = 150.f;

	UPROPERTY(EditInstanceOnly, Category = AI)
	AActor* CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = AI)
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = AI)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = AI)
	double WalkSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = AI)
	double RunSpeed = 300.f;

	FTimerHandle PatrolTimer;

	UPROPERTY()
	class AAIController* EnemyController;


	/** COMBAT */

	/// <summary>
	/// Sets the attack timer so the enemy waits a few seconds before attacking again
	/// </summary>
	void StartAttackTimer();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttacDelayMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDelayMax = 1.f;
};
