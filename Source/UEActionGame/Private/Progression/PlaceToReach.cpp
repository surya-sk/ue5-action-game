// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/PlaceToReach.h"
#include "Components/SphereComponent.h"

// Sets default values
APlaceToReach::APlaceToReach()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APlaceToReach::BeginPlay()
{
	Super::BeginPlay();
	

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &APlaceToReach::OnSphereOverlap);
}

void APlaceToReach::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OnPlaceReached.IsBound() && OtherActor->ActorHasTag("PlayerCharacter"))
	{
		OnPlaceReached.Broadcast();
	}
}

// Called every frame
void APlaceToReach::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

