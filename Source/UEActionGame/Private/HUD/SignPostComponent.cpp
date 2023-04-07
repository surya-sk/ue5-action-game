// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SignPostComponent.h"
#include "HUD/SignWidget.h"
#include "Components/TextBlock.h"

void USignPostComponent::SetSignText(FText SignText)
{
	if (SignPostWidget == nullptr)
	{
		SignPostWidget = Cast<USignWidget>(GetUserWidgetObject());
	}

	if (SignPostWidget && SignPostWidget->SignTextBlock)
	{
		SignPostWidget->SignTextBlock->SetText(SignText);
	}
}
