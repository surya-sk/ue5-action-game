// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mission.h"
#include "Quest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveUpdated);

UCLASS()
class UEACTIONGAME_API AQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuest();

	virtual void Tick(float DeltaTime) override;
	inline FText GetCurrentObjective() const { return Objectives[ActiveObjectiveIndex]->GetText(); }

	inline int32 GetActiveObjectiveIndex() const { return ActiveObjectiveIndex; }

	inline void SetActiveObjectiveIndex(int32 ActiveObjective) { ActiveObjectiveIndex = ActiveObjective; }

	FObjectiveUpdated OnObjectiveUpdated;

	void InitObjectives();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly)
	TArray<AMission*> Objectives;

	UPROPERTY(EditAnywhere)
	FName NextMap;

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

	int32 ActiveObjectiveIndex = 0;

};
