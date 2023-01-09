// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEACTIONGAME_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReceieveDamage(float Damage);
	void ConsumeStamina(float StaminaCost);
	void HandleStamina(bool bSprinting);
	bool HasEnoughStamina();
	void HandleOxygen(bool bSwimming);
	FORCEINLINE void RegenrateOxygen() { Oxygen = MaxOxygen; }
	void RegenrateHealth();
	bool IsDead() const;

	FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; }
	FORCEINLINE float GetStaminaPercent() const { return Stamina / MaxStamina; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(EditAnywhere)
	float HealthRegenRate;

	UPROPERTY(EditAnywhere)
	float Stamina;

	UPROPERTY(EditAnywhere)
	float MaxStamina;

	UPROPERTY(EditAnywhere)
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere)
	float StaminaRegenRate;

	UPROPERTY(EditAnywhere)
	float Oxygen;

	UPROPERTY(EditAnywhere)
	float MaxOxygen;

	UPROPERTY(EditAnywhere);
	float OxygenDrainRate;

	UPROPERTY(EditAnywhere)
	class USoundBase* TiredSound;
};
