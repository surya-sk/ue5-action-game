// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/GhostTrigger.h"
#include "Components/BoxComponent.h"
#include "Enemy/Ghost.h"
#include "Characters/PresentDayCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGhostTrigger::AGhostTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGhostTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter)
	{
		MainCharacter = Cast<APresentDayCharacter>(PlayerCharacter);
	}

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AGhostTrigger::OnBoxOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AGhostTrigger::OnBoxEndOverlap);

	if (GhostToSpawn)
	{
		GhostToSpawn->SetActorHiddenInGame(true);
		GhostToSpawn->SetGhostAction(GhostAction);
	}
}

void AGhostTrigger::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<APresentDayCharacter>())
	{
		GhostToSpawn->SetActorHiddenInGame(false);
	}
}

void AGhostTrigger::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void AGhostTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

