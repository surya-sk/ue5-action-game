// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMapName : uint8
{
	Ravenswood UMETA(DisplayName = "Past Ravenswood"),
	Demo_Ravenswood_Present UMETA(DisplayName = "Present Ravenswood")
};

class FMapUtils
{
public:
	static FName GetMapName(EMapName Map);
};