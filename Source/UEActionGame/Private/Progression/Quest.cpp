// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/Quest.h"
#include "Progression/Mission.h"
#include "Kismet/GameplayStatics.h"
#include "Progression/SaveSystem.h"

// Sets default values
AQuest::AQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AQuest::InitObjectives()
{
	UE_LOG(LogTemp, Warning, TEXT("Init objectives"));
	UE_LOG(LogTemp, Warning, TEXT("Objective count: %d"), Objectives.Num());
	UE_LOG(LogTemp, Warning, TEXT("Active objective index: %d"), Objectives.Num());
	ActivateNewObjective();
}

// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin play"));
	
}

void AQuest::ActivateNewObjective()
{
	Objectives[ActiveObjectiveIndex]->Activate();
	Objectives[ActiveObjectiveIndex]->OnMissionFinished.AddDynamic(this, &AQuest::EndCurrentObjective);
	if (OnObjectiveUpdated.IsBound())
	{
		OnObjectiveUpdated.Broadcast();
	}
}

void AQuest::EndCurrentObjective()
{
	if (ActiveObjectiveIndex + 1 < Objectives.Num())
	{
		ActiveObjectiveIndex++;
		ActivateNewObjective();
	}
	else
	{
		if (NextMap != "")
		{
			UGameplayStatics::OpenLevel(this, NextMap);
		}
	}
}

// Called every frame
void AQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

