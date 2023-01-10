// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoteWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API UNoteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNoteText(FText Text);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NoteText;
};
