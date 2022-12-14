// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mission.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMissionFinished);

UCLASS()
class UEACTIONGAME_API AMission : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMission();

	/// <summary>
	/// Activate the current mission
	/// </summary>
	void Activate();

	/// <summary>
	/// Mark the current mission as complete
	/// </summary>
	void Complete();

	FORCEINLINE FText GetText() const { return ObjectiveText; }

	FMissionFinished OnMissionFinished;

protected:
	/// <summary>
	/// Complete the mission on killing the enemy
	/// </summary>
	UFUNCTION()
	void EnemyKilled();

	/// <summary>
	/// Complete the mission on reaching the destination
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
	void PlaceReached(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// Complete the mission on finding the item
	/// </summary>
	UFUNCTION()
	void ItemFound();

	UPROPERTY(EditInstanceOnly)
	FText ObjectiveText;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> ActorsToActivate;

	UPROPERTY(EditInstanceOnly)
	class AEnemy* EnemyToKill;

	UPROPERTY(EditInstanceOnly)
	class UBoxComponent* PlaceToReach;

	UPROPERTY(EditInstanceOnly)
	class AExpositionNote* ItemToFind;

};
