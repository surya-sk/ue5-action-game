// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ExpositionNote.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "HUD/NoteWidget.h"

void AExpositionNote::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	if (NoteWidget)
	{
		FString String = NoteText.ToString();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *String);
		NoteWidget->AddToViewport();
		NoteWidget->SetNoteText(NoteText);
	}
}

void AExpositionNote::Unequip()
{
	if (OnNoteRead.IsBound())
	{
		OnNoteRead.Broadcast();
	}
	if (NoteWidget)
	{
		NoteWidget->RemoveFromViewport();
	}
}
