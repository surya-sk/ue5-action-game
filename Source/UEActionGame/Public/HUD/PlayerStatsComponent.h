// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PlayerStatsComponent.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class UEACTIONGAME_API UPlayerStatsComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;
};
