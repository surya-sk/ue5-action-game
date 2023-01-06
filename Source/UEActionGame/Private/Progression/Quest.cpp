// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/Quest.h"
#include "Progression/Mission.h"

// Sets default values
AQuest::AQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
	Super::BeginPlay();
	if (Objectives.Num() > 0)
	{
		ActivateNewObjective();
	}
	
}

void AQuest::ActivateNewObjective()
{
	Objectives[ActiveObjectiveIndex]->Activate();
	Objectives[ActiveObjectiveIndex]->OnMissionFinished.AddDynamic(this, &AQuest::EndCurrentObjective);
}

void AQuest::EndCurrentObjective()
{
	UE_LOG(LogTemp, Warning, TEXT("Objective complete"));
	if (ActiveObjectiveIndex < Objectives.Num())
	{
		ActiveObjectiveIndex++;
		ActivateNewObjective();
	}
}

// Called every frame
void AQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

