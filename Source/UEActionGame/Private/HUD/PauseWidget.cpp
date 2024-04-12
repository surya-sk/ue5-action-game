// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PauseWidget.h"
#include "Components/Button.h"
#include "Characters/MainCharacter.h"
#include "Characters/PresentDayCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseWidget::SetFocusOnButtons()
{
	ResumeButton->SetFocus();
	ResumeButton->SetKeyboardFocus();
}

void UPauseWidget::Resume()
{
	if (PastCharacter == nullptr && PresentCharacter == nullptr)
	{
		GetCharacterReferences();
	}

	if (PastCharacter)
	{
		PastCharacter->HidePauseMenu();
	}
	else
	{
		PresentCharacter->HidePauseMenu();
	}
}

void UPauseWidget::SaveGame()
{
	if (PastCharacter == nullptr && PresentCharacter == nullptr)
	{
		GetCharacterReferences();
	}

	if (PastCharacter)
	{
		PastCharacter->SaveGame();
	}
	else
	{
		PresentCharacter->SaveGame();
	}
	Resume();
}

void UPauseWidget::LoadGame()
{
	if (PastCharacter == nullptr && PresentCharacter == nullptr)
	{
		GetCharacterReferences();
	}

	if (PastCharacter)
	{
		PastCharacter->LoadGame();
	}
	else
	{
		PresentCharacter->LoadGame();
	}
	Resume();
}

void UPauseWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 
		UGameplayStatics::GetPlayerController(GetWorld(), 0), 
		EQuitPreference::Quit, false);
}

void UPauseWidget::GetCharacterReferences()
{
	APawn* Pawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	if (Pawn)
	{
		PastCharacter = Cast<AMainCharacter>(Pawn);
		if (IsValid(PastCharacter))
		{
			return;
		}
		PresentCharacter = Cast<APresentDayCharacter>(Pawn);
	}
}
