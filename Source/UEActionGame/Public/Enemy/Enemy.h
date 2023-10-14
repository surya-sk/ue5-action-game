// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyKilled);

UCLASS()
class UEACTIONGAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	void PlayAssassinationMontage();

	FORCEINLINE void SetTurnToAttack(bool bShouldAttack) { this->bIsTurnToAttack = bShouldAttack; }

	FEnemyKilled OnEnemyKilled;

protected:
	virtual void BeginPlay() override;

	/** <ABaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	/** </ABaseCharacter> */

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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
	bool IsInTargetRange(FVector Target, double AcceptanceRadius);

	/// <summary>
	/// Moves the enemy to the Target
	/// </summary>
	/// <param name="Target">The actor to move towards</param>
	void MoveToTarget(FVector Target);

	/// <summary>
	/// Chooses a patrol target at random
	/// </summary>
	/// <returns>The patrol target</returns>
	FVector ChoosePatrolTarget();

	/// <summary>
	/// Callback for OnPawnSeen for PawnSensing
	/// </summary>
	/// <param name="SeenPawn">The pawn that has been seen</param>
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void TorchSeen(APawn* SeenPawn);

	/// <summary>
	/// Alerts enemies nearby that the player has been seen
	/// </summary>
	void AlertNearbyEnemies();

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* TorchPawnSensing;

	UPROPERTY(EditAnywhere, Category = AI)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = AI)
	double AttackRadius = 150.f;

	UPROPERTY(EditInstanceOnly, Category = AI)
	FVector CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = AI)
	TArray<AActor*> PatrolPointActors; // These are just so it's easy to drop in actors to the details panel

	UPROPERTY(EditAnywhere, Category = AI)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = AI)
	double WalkSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = AI)
	double RunSpeed = 300.f;

	FTimerHandle PatrolTimer;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* AssassinationBox;


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

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AssassinationMontage;

	UPROPERTY(EditAnywhere, Category = AI)
	float ReachablePatrolRadius;

	TArray<FVector> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = AI)
	bool bShouldPatrol = true;

	UPROPERTY(EditAnywhere)
	class USphereComponent* NearbyEnemyTrigger;

	TArray<AEnemy*> NearbyEnemies;

	UPROPERTY(EditAnywhere)
	class USoundBase* AlertSound;

	bool bChasing;

	bool bIsTurnToAttack;

	class EnemyCoordinator* Coordinator;
};
