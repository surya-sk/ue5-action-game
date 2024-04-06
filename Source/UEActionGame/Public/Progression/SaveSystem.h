// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveSystem.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API USaveSystem : public USaveGame
{
	GENERATED_BODY()
public:
	USaveSystem();

	UPROPERTY(VisibleAnywhere, Category=Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category=Basic)
	uint32 UserIndex;
};
