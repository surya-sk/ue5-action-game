// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerStatsComponent.h"
#include "Components/ProgressBar.h"

void UPlayerStatsComponent::SetHealthBarPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UPlayerStatsComponent::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}
