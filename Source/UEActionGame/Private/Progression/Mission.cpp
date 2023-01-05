// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/Mission.h"
#include "Enemy/Enemy.h"

// Sets default values
AMission::AMission()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMission::Activate()
{
	if (EnemyToKill)
	{
		EnemyToKill->OnEnemyKilled.AddDynamic(this, &AMission::EnemyKilled);
	}
}

void AMission::Complete()
{
	if (OnMissionFinished.IsBound())
	{
		OnMissionFinished.Broadcast();
	}
}

void AMission::EnemyKilled()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy killed"));
	Complete();
}


