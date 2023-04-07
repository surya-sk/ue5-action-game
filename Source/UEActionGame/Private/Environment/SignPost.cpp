// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/SignPost.h"
#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"
#include "HUD/SignPostComponent.h"

// Sets default values
ASignPost::ASignPost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SignMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignMesh"));
	SetRootComponent(SignMesh);

	SignWidget = CreateDefaultSubobject<USignPostComponent>(TEXT("SignWidget"));
	SignWidget->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASignPost::BeginPlay()
{
	Super::BeginPlay();

	if (SignWidget)
	{
		SignWidget->SetSignText(SignText);
	}
	
}

// Called every frame
void ASignPost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

