// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/NoteWidget.h"
#include "Components/TextBlock.h"

void UNoteWidget::SetNoteText(FText Text)
{
	if (NoteText)
	{
		UE_LOG(LogTemp, Warning, TEXT("NoteText"));
		NoteText->SetText(Text);
	}
}
