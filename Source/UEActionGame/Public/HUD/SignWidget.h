// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API USignWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SignTextBlock;

	
};
