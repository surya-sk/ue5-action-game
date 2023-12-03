// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostTrigger.generated.h"

UENUM(BlueprintType)
enum class EGhostAction
{
	EGA_AppearAndDisappear UMETA(DisplayName = "Appear and Disappear"),
	EGA_WalkBy UMETA(DisplayName = "Walk By"),
	EGA_Patrol UMETA(DisplayName = "Patrol")
};

UCLASS()
class UEACTIONGAME_API AGhostTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostTrigger();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere)
	class AGhost* GhostToSpawn;

private:
	class APresentDayCharacter* MainCharacter;
};
