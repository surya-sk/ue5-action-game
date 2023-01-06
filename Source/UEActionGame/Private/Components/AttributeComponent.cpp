// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::ReceieveDamage(float Damage)
{
	Health = FMath::Clamp<float>(Health - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::ConsumeStamina(float StaminaCost)
{
	Stamina = FMath::Clamp<float>(Stamina - StaminaCost, 0.f, MaxStamina);
}

void UAttributeComponent::HandleStamina(bool bSprinting)
{
	if ((Stamina - StaminaDrainRate) > 0.f && bSprinting)
	{
		Stamina -= StaminaDrainRate;
	}
	else
	{
		if (TiredSound && !HasEnoughStamina())
		{
			UGameplayStatics::PlaySound2D(GetWorld(), TiredSound);
		}
		if (Stamina < MaxStamina)
		{
			Stamina += StaminaRegenRate;
		}
	}
}

bool UAttributeComponent::HasEnoughStamina()
{
	return Stamina > 1.f;
}

void UAttributeComponent::RegenrateHealth()
{
	if (Health < MaxHealth)
	{
		Health += HealthRegenRate;
	}
}

bool UAttributeComponent::IsDead() const
{
	return Health <= 0.f;
}


