// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsClosed = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	OpenTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("OpenTrigger"));
	OpenTrigger->SetupAttachment(RootComponent);

	CloseTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CloseTrigger"));
	CloseTrigger->SetupAttachment(RootComponent);

	OpenRotationValue = 90.f;
	CloseRotationValue = 0.f;
	CurrentRotation = CloseRotationValue;
	TargetRotation = CloseRotationValue;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	OpenTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOpenTriggerOverlapBegin);
	CloseTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnCloseTriggerOverlapBegin);
}

void ADoor::OnOpenTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("PlayerCharacter")))
	{
		TargetRotation = OpenRotationValue;
		if (OpenSound)
			UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
	}
}

void ADoor::OnCloseTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("PlayerCharacter")))
	{
		TargetRotation = CloseRotationValue;
		if (CloseSound)
			UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
	}
}

void ADoor::RotateDoor(float DeltaTime)
{
	if (CurrentRotation != TargetRotation)
	{
		CurrentRotation = FMath::FInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);
		Mesh->SetRelativeRotation(FRotator(0.f, CurrentRotation, 0.f));
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateDoor(DeltaTime);
}

