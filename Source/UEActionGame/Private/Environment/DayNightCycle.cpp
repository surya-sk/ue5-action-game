// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/DayNightCycle.h"
#include "Engine/DirectionalLight.h"
#include "Misc/OutputDeviceNull.h"

// Sets default values
ADayNightCycle::ADayNightCycle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADayNightCycle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADayNightCycle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LightSource)
	{
		LightSource->AddActorLocalRotation(FRotator(0.f, DeltaTime * TurnRate, 0.f));
		bIsNight = LightSource->GetActorRotation().Pitch > 2.0;
	}

	if (Sun)
	{
		FOutputDeviceNull OD;
		Sun->CallFunctionByNameWithArguments(TEXT("UpdateSunDirection"), OD, NULL, true);
	}
}

