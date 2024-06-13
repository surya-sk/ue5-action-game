// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/MapNames.h"

FName FMapUtils::GetMapName(EMapName Map)
{
	FName Result;
	if (Map == EMapName::Demo_Ravenswood_Present)
	{
		Result = FName("Demo_Ravenswood_Present");
	}
	else
	{
		Result = FName("Ravenswood");
	}
	return Result;
}
