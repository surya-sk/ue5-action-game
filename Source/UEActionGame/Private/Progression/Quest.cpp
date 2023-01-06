// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/Quest.h"
#include "Progression/Mission.h"

// Sets default values
AQuest::AQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
	Super::BeginPlay();
	if (Objectives.Num() > 0)
	{
		Objectives[ActiveObjectiveIndex]->Activate();
		Objectives[ActiveObjectiveIndex]->OnMissionFinished.AddDynamic(this, &AQuest::EndCurrentObjective);
	}
	
}

void AQuest::ActivateObjective()
{
}

void AQuest::EndCurrentObjective()
{
	UE_LOG(LogTemp, Warning, TEXT("Objective complete"));
}

// Called every frame
void AQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

