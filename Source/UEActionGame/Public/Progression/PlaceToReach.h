// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceToReach.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlaceReached);

UCLASS()
class UEACTIONGAME_API APlaceToReach : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceToReach();

	virtual void Tick(float DeltaTime) override;

	FPlaceReached OnPlaceReached;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	class USphereComponent* Sphere;



};
