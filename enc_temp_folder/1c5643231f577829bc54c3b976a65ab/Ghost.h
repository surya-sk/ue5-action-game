// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Ghost.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGhostKilled);

UCLASS()
class UEACTIONGAME_API AGhost : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AGhost();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetHit(const FVector& ImpactPoint, AActor* Hitter) override;

	FORCEINLINE void SetGhostAction(EGhostAction GhostAction) { Action = GhostAction; }

	FGhostKilled OnGhostKilled;

protected:
	virtual void BeginPlay() override;

	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;

	UPROPERTY(BlueprintReadOnly)
	EGhostState State = EGhostState::EGS_Patrolling;
	
	UPROPERTY(EditAnywhere)
	float DisappearIn = 5.0f;

private:
	void Disappear();

	EGhostAction Action;

	FTimerHandle DestroyTimer;

};
