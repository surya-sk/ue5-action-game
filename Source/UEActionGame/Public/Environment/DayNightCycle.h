// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/OutputDeviceNull.h"
#include "GameFramework/Actor.h"
#include "DayNightCycle.generated.h"

UCLASS()
class UEACTIONGAME_API ADayNightCycle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayNightCycle();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE bool IsNight() const { return bIsNight; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = Sky)
	AActor* Sun;

	UPROPERTY(EditInstanceOnly, Category = Sky)
	class ADirectionalLight* LightSource;

	UPROPERTY(EditInstanceOnly, Category = Sky)
	float TurnRate;

private:
	bool bIsNight;
	

};
