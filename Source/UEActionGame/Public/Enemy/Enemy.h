// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class UEACTIONGAME_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;

	void DirectionalHitReact(const FVector& ImpactPoint);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Play montage functions
	/// </summary>
	void PLayHitReactMontage(const FName SectionName);

private:
	/// <summary>
	/// Animation montages
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* HitReactMontage;


	UPROPERTY(EditAnywhere, Category = Effects)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = Effects)
	UParticleSystem* HitParticles;

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

};
