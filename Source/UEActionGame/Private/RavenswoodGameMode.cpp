// Fill out your copyright notice in the Description page of Project Settings.


#include "RavenswoodGameMode.h"
#include "Progression/Quest.h"
#include "Kismet/GameplayStatics.h"

APlayerController* ARavenswoodGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	if (Quest)
	{
		return Super::SpawnPlayerController(InRemoteRole, Options);
	}
	else
	{
		CheckQuestActor();
		return SpawnPlayerController(InRemoteRole, Options);
	}
	return nullptr;
}

void ARavenswoodGameMode::StartPlay()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ARavenswoodGameMode::CheckQuestActor);
	Super::StartPlay();
}

void ARavenswoodGameMode::CheckQuestActor()
{
	if (Quest == nullptr)
	{
		Quest = Cast<AQuest>(UGameplayStatics::GetActorOfClass(GetWorld(), AQuest::StaticClass()));
	}
}
