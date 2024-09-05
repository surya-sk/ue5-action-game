// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Progression/MapNames.h"
#include "TimeJump.generated.h"

UCLASS()
class UEACTIONGAME_API ATimeJump : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeJump();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SwitchTimePeriod(const UObject* WorldContextObject, EMapName MapToNavigate, float TimeDelay, bool bLoadPosition = true);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle DelayHandle;
	void LoadMap();

	EMapName MapToNavigate = EMapName::Ravenswood;

	UWorld* CurrentWorld;
};
