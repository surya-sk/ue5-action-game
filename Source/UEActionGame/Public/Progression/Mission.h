// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mission.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMissionFinished);

UCLASS()
class UEACTIONGAME_API AMission : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMission();

	void Activate();
	void Complete();

	FMissionFinished OnMissionFinished;

protected:
	UFUNCTION()
	void EnemyKilled();

	UPROPERTY(EditInstanceOnly)
	FName ObjectiveText;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> ActorsToActivate;

	UPROPERTY(EditInstanceOnly)
	class AEnemy* EnemyToKill;

	UPROPERTY(EditInstanceOnly)
	class UBoxComponent* PlaceToReach;

	UPROPERTY(EditInstanceOnly)
	class AItem* ItemToFind;

};
