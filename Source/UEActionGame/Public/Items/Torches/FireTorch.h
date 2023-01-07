// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "FireTorch.generated.h"
/**
 * 
 */
UCLASS()
class UEACTIONGAME_API AFireTorch : public AItem
{
	GENERATED_BODY()

public:
	AFireTorch();
	virtual void BeginPlay() override;
	void Unequip();

protected:
	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* FireParticles;

	UPROPERTY(EditAnywhere)
	class USoundBase* FireSound;
};
