// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SignPostWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API USignPostWidget : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetSignText(FText SignText);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SignTextBlock;
	
};
