// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/MainCharacter.h"
#include "Dialogue/DialogueManager.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);

	DialogueText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DialogueText"));
	DialogueText->SetupAttachment(Mesh);

	CurrentLineIndex = -1;
}

void ANPC::Interact()
{
	if (DialogueKeys.Num() == 0)
		return;

	if (CurrentLineIndex == -1)
	{
		CurrentLineIndex = 0;
		DialogueText->SetText(FText::FromString(DialogueManager->GetDialogueLine(DialogueKeys[CurrentLineIndex])));
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
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnSphereEndOverlap);

	if (DialogueManager == nullptr)
		UE_LOG(LogTemp, Error, TEXT("Dialogue Manager missing!"));
}

void ANPC::NextLine()
{
	CurrentLineIndex++;

	if (CurrentLineIndex < DialogueKeys.Num())
	{
		bool bPlayerLine = CurrentLineIndex % 2 != 0;
		FText DialogueLine = FText::FromString(DialogueManager->GetDialogueLine(DialogueKeys[CurrentLineIndex]));
		if (bPlayerLine)
			Player->SetDialogueText(DialogueLine);
		else
			DialogueText->SetText(DialogueLine);
	}
	else
	{
		CurrentLineIndex = -1;
		DialogueText->SetText(FText::GetEmpty());
	}
}

void ANPC::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AMainCharacter>(OtherActor);
	if (Player)
	{
		Player->SetDialogueState(true, this);
	}
}

void ANPC::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Player)
	{
		Player->SetDialogueState(false);
	}
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
