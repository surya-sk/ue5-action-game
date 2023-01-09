// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "HUD/PlayerOverlay.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && PlayerOverlayClass)
		{
			UPlayerOverlay* Overlay = CreateWidget<UPlayerOverlay>(Controller, PlayerOverlayClass);
			Overlay->AddToViewport();
		}
	}
}
