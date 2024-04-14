// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/TimeJump.h"
#include "Components/BoxComponent.h"

// Sets default values
ATimeJump::ATimeJump()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TimeJumpBox"));
	Trigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATimeJump::BeginPlay()
{
	Super::BeginPlay();
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATimeJump::OnBoxOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ATimeJump::OnBoxEndOverlap);
}

void ATimeJump::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ATimeJump::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void ATimeJump::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

