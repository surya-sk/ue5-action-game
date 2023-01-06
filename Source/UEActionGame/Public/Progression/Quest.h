// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quest.generated.h"

class UObjective;

UCLASS()
class UEACTIONGAME_API AQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuest();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly)
	TArray<class AMission*> Objectives;

private:
	/// <summary>
	/// Activates the new objective
	/// </summary>
	void ActivateNewObjective();

	/// <summary>
	/// Ends the current mission and moves on the next one
	/// </summary>
	UFUNCTION()
	void EndCurrentObjective();

	int ActiveObjectiveIndex = 0;

};
