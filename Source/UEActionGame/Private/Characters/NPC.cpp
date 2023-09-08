// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC.h"
#include "Components/TextRenderComponent.h"
#include "Characters/MainCharacter.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DialogueText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DialogueText"));
	DialogueText->SetupAttachment(RootComponent);

	CurrentLineIndex = -1;
}

void ANPC::Interact()
{
	if (DialogueLines.Num() == 0)
		return;

	if (CurrentLineIndex == -1)
	{
		CurrentLineIndex = 0;
		DialogueText->SetText(FText::FromString(DialogueLines[CurrentLineIndex]));
	}
	else
	{
		NextLine();
	}
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPC::NextLine()
{
	CurrentLineIndex++;

	if (CurrentLineIndex < DialogueLines.Num())
	{
		DialogueText->SetText(FText::FromString(DialogueLines[CurrentLineIndex]));
	}
	else
	{
		CurrentLineIndex = -1;
		DialogueText->SetText(FText::GetEmpty());
	}
}

void ANPC::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("PlayerCharacter"))
	{
		AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
		if (Player)
		{
			Player->SetDialogueState(true, this);
		}
	}
}

void ANPC::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("PlayerCharacter"))
	{
		AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
		if (Player)
		{
			Player->SetDialogueState(false);
		}
	}
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
