// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Water.h"

// Sets default values
AWater::AWater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWater::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWater::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

// Called every frame
void AWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

