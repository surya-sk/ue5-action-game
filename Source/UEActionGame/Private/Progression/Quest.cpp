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
	if (Objectives.Num() > 0)
	{
		if (auto* SaveSystem = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(USaveGame::StaticClass())))
		{
			SaveSystem = Cast<USaveSystem>(UGameplayStatics::LoadGameFromSlot(SaveSystem->PlayerName, SaveSystem->UserIndex));
			auto* PastCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			if (PastCharacter)
			{
				if (SaveSystem->PlayerData.PastCurrentObjectiveIndex >= 0)
				{
					ActiveObjectiveIndex = SaveSystem->PlayerData.PastCurrentObjectiveIndex;
				}
			}
			else
			{
				auto* PresentCharacter = Cast<APresentDayCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
				if (SaveSystem->PlayerData.PresentCurrentObjectiveIndex >= 0)
				{
					ActiveObjectiveIndex = SaveSystem->PlayerData.PresentCurrentObjectiveIndex;
				}
			}
		}

		ActivateNewObjective();
	}
	
}

void AQuest::ActivateNewObjective()
{
	UE_LOG(LogTemp, Warning, TEXT("New Objective"));
	Objectives[ActiveObjectiveIndex]->Activate();
	auto* PastCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PastCharacter)
	{
		PastCharacter->CurrentObjectiveIndex = ActiveObjectiveIndex;
	}
	else
	{
		auto* PresentCharacter = Cast<APresentDayCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		PresentCharacter->CurrentObjectiveIndex = ActiveObjectiveIndex;
	}
	Objectives[ActiveObjectiveIndex]->OnMissionFinished.AddDynamic(this, &AQuest::EndCurrentObjective);
	if (OnObjectiveUpdated.IsBound())
	{
		OnObjectiveUpdated.Broadcast();
	}
}

void AQuest::EndCurrentObjective()
{
	UE_LOG(LogTemp, Warning, TEXT("Objective complete"));
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

