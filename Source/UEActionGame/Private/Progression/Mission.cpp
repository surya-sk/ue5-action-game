// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/Mission.h"
#include "Progression/PlaceToReach.h"
#include "Enemy/Enemy.h"
#include "Items/ExpositionNote.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/MainCharacter.h"

// Sets default values
AMission::AMission()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AMission::Activate()
{
	if (bForcePlayerFollow)
	{
		if (Player == nullptr)
		{
			Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		}

		if (Player)
			Player->SetFollowState(true);
		else
			UE_LOG(LogTemp, Error, TEXT("Player is null"));
	}

	if (EnemyToKill)
	{
		EnemyToKill->OnEnemyKilled.AddDynamic(this, &AMission::EnemyKilled);
	}
	else if(PlaceToReach)
	{
		PlaceToReach->OnPlaceReached.AddDynamic(this, &AMission::PlaceReached);
	}
	else if (ItemToFind)
	{
		ItemToFind->OnNoteRead.AddDynamic(this, &AMission::ItemFound);
	}
}

void AMission::Complete()
{
	if (bForcePlayerFollow)
	{
		Player->SetFollowState(false);
	}
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


void AMission::PlaceReached()
{
	UE_LOG(LogTemp, Warning, TEXT("Place reached"));
	Complete();
}

void AMission::ItemFound()
{
	UE_LOG(LogTemp, Warning, TEXT("Item found"));
	Complete();
}


