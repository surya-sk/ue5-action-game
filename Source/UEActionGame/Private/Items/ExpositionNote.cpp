// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ExpositionNote.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "HUD/NoteWidget.h"

void AExpositionNote::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	Super::Equip(InParent, InSocketName, NewOwner, NewInstigator);
	if (NoteWidget)
	{
		FString String = NoteText.ToString();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *String);
		NoteWidget->SetNoteText(NoteText);
		NoteWidget->AddToViewport();
	}
}

void AExpositionNote::Unequip()
{

	if (OnNoteRead.IsBound())
	{
		OnNoteRead.Broadcast();
	}
	NoteWidget->RemoveFromViewport();
}
