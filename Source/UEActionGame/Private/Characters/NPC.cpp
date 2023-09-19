// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/MainCharacter.h"
#include "Dialogue/DialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMeshComponent* NPCMesh = this->GetMesh();

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(NPCMesh);

	DialogueText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DialogueText"));
	DialogueText->SetupAttachment(NPCMesh);

	CurrentKeyIndex = -1;

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
	CurrentKeyIndex++;

	if (CurrentKeyIndex < DialogueKeys.Num())
	{
		bool bPlayerLine = CurrentKeyIndex % 2 != 0;
		DialogueData = DialogueManager->GetDialogueData(DialogueKeys[CurrentKeyIndex]);
		FText DialogueLine = FText::FromString(DialogueData.DialogueLine);
		USoundBase* DialogAudio = DialogueData.DialogueAudio;
		if (bPlayerLine)
		{
			Player->SetDialogueText(DialogueLine);
			Player->PlayDialogueAudio(DialogAudio);
		}
		else
		{
			DialogueText->SetText(DialogueLine);
			PlayDialogueAudio(DialogAudio);
		}
	}
	else
	{
		CurrentKeyIndex = -1;
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

void ANPC::PlayDialogueAudio(USoundBase* DialogueAudio)
{
	if (DialogueAudio)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), DialogueAudio);
		PlayRandomDialogueAnimation();
	}
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::Interact()
{
	if (DialogueKeys.Num() == 0)
		return;

	if (CurrentKeyIndex == -1)
	{
		CurrentKeyIndex = 0;
		DialogueData = DialogueManager->GetDialogueData(DialogueKeys[CurrentKeyIndex]);
		DialogueText->SetText(FText::FromString(DialogueData.DialogueLine));
		PlayDialogueAudio(DialogueData.DialogueAudio);
	}
	else
	{
		NextLine();
	}
}


