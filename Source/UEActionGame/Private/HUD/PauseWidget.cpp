// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PauseWidget.h"
#include "Components/Button.h"

void UPauseWidget::GetButtons(TArray<UButton*>& Buttons)
{
	Buttons.Add(ResumeButton);
	Buttons.Add(SaveButton);
	Buttons.Add(LoadButton);
	Buttons.Add(QuitButton);
}

void UPauseWidget::SetFocusOnButtons(APlayerController* PC)
{
	ResumeButton->SetFocus();
	ResumeButton->SetKeyboardFocus();
}
