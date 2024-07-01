// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RavenswoodGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API ARavenswoodGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;
	virtual void StartPlay() override;

private:
	void CheckQuestActor();

	class AQuest* Quest;
};
