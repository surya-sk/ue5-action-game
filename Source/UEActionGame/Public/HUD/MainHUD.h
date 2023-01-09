// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UPlayerOverlay;

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() const { return Overlay; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	UPlayerOverlay* Overlay;
	
};
