// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SignPostWidget.h"
#include "Components/TextBlock.h"

void USignPostWidget::SetSignText(FText SignText)
{
	if (SignTextBlock)
	{
		SignTextBlock->SetText(SignText);
	}
}
