// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONGAME_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetFocusOnButtons();

protected:
	UFUNCTION(BlueprintCallable)
	void Resume();
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();
	UFUNCTION(BlueprintCallable)
	void QuitGame();
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	class AMainCharacter* PastCharacter;

	class APresentDayCharacter* PresentCharacter;

	void GetCharacterReferences();
};
