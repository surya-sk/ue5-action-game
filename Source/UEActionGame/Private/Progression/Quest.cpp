// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/Quest.h"
#include "Progression/Mission.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/MainCharacter.h"
#include "Characters/PresentDayCharacter.h"
#include "Progression/SaveSystem.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Quest BeginPlay"));

	if (Objectives.Num() > 0)
	{
		ActivateNewObjective();
	}
	
}

void AQuest::ActivateNewObjective()
{
	Objectives[ActiveObjectiveIndex]->Activate();
	if (auto* PastCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		PastCharacter->CurrentObjectiveIndex = ActiveObjectiveIndex;
	}
	else if(auto* PresentCharacter = Cast<APresentDayCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		PresentCharacter->CurrentObjectiveIndex = ActiveObjectiveIndex;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No character found"));
	}
	Objectives[ActiveObjectiveIndex]->OnMissionFinished.AddDynamic(this, &AQuest::EndCurrentObjective);
	if (OnObjectiveUpdated.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("Objective delegate bound"));
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

