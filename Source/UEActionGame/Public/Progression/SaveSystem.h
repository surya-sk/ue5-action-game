// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveSystem.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	bool bWeaponEquipped;

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	FString LastMapName;

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category="SaveGameData")
	FRotator Rotation;
};

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

	UPROPERTY(VisibleAnywhere, Category=Basic)
	FPlayerData PlayerData;
};
