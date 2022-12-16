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
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckPatrolTarget();

	void CheckCombatTarget();

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void HandleDamage(float DamageAmount) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Die() override;

	bool IsInTargetRange(AActor* Target, double AcceptanceRadius);

	void MoveToTarget(AActor* Target);

	AActor* ChoosePatrolTarget();

	virtual void Attack() override;

	virtual bool CanAttack() override;

	virtual int32 PlayDeathMontage() override;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
	void PatrolTimerFinished();
	void StartPatrolling();
	void ChaseTarget();

	void StartAttackTimer();

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttacDelaykMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDelayMax = 1.f;
};
