// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/Objective.h"
#include "Enemy/Enemy.h"

void UObjective::Activate()
{
	// TODO: Activate Actors
	if (EnemyToKill)
	{
		EnemyToKill->OnEnemyKilled.AddDynamic(this, &UObjective::EnemyKilled);
	}
}

void UObjective::Complete()
{
}

void UObjective::EnemyKilled()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy killed"));
}
