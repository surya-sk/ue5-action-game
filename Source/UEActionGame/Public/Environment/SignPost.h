// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SignPost.generated.h"

UCLASS()
class UEACTIONGAME_API ASignPost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASignPost();
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FText SignText;

	UPROPERTY(EditAnywhere)
	class USignPostComponent* SignWidget;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SignMesh;

};
