// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class UEACTIONGAME_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* CloseTrigger;

	UPROPERTY(VisibleAnywhere,  BlueprintReadWrite)
	class UBoxComponent* OpenTrigger;

	UPROPERTY(EditAnywhere)
	USoundBase* OpenSound;

	UPROPERTY(EditAnywhere)
	USoundBase* CloseSound;

	UFUNCTION()
	void OnOpenTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void  OnCloseTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bIsClosed;
	float CurrentRotation;
	float TargetRotation;
	float OpenRotationValue;
	float CloseRotationValue;

	void RotateDoor(float DeltaTime);

};
