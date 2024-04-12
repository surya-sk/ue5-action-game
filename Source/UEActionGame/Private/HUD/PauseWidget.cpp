// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PauseWidget.h"
#include "Components/Button.h"

void UPauseWidget::SetFocusOnButtons()
{
	ResumeButton->SetFocus();
	ResumeButton->SetKeyboardFocus();
}
