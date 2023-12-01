// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
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

	FGhostKilled OnGhostKilled;

protected:
	virtual void BeginPlay() override;

};
