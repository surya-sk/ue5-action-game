// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/TimeJump.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATimeJump::ATimeJump()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimeJump::BeginPlay()
{
	Super::BeginPlay();
}

void ATimeJump::SwitchTimePeriod(FName& InMapToNavigate, float InTimeDelay)
{
	MapToNavigate = InMapToNavigate;
	GetWorldTimerManager().SetTimer(DelayHandle, this, &ATimeJump::LoadMap, InTimeDelay, false);
}

void ATimeJump::LoadMap()
{
	UGameplayStatics::OpenLevel(this, MapToNavigate, true, "LoadAsync");
}

// Called every frame
void ATimeJump::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

