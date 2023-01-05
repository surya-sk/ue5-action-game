// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Objective.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveFinished);

UCLASS(Blueprintable)
class UEACTIONGAME_API UObjective : public UObject
{
	GENERATED_BODY()

public:
	void Activate();
	void Complete();

	FObjectiveFinished OnObjectiveFinished;

protected:
	UFUNCTION()
	void EnemyKilled();

	UPROPERTY(EditAnywhere)
	FName ObjectiveText;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToActivate;

	UPROPERTY()
	class AEnemy* EnemyToKill;

	UPROPERTY()
	class UBoxComponent* PlaceToReach;

	UPROPERTY()
	class AItem* ItemToFind;
	
};
