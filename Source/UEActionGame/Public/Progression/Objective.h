// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Objective.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API UObjective : public UObject
{
	GENERATED_BODY()

public:
	void Activate();

protected:
	UPROPERTY(EditAnywhere)
	FName ObjectiveText;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToActivate;

	UPROPERTY(EditAnywhere)
	class AEnemy* EnemyToKill;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* PlaceToReach;

	UPROPERTY(EditAnywhere)
	class AItem* ItemToFind;
	
};
