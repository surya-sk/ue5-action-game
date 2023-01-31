// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SignPostComponent.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API USignPostComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetSignText(FText SignText);

protected:
	UPROPERTY(VisibleAnywhere)
	class USignWidget* SignPostWidget;
	
};
