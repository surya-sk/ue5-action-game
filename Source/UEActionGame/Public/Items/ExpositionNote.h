// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "ExpositionNote.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoteRead);

UCLASS()
class UEACTIONGAME_API AExpositionNote : public AItem
{
	GENERATED_BODY()

public:
	FNoteRead OnNoteRead;
	
	virtual void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator) override;

	void Unequip();

private:
	UPROPERTY(EditDefaultsOnly)
	class UNoteWidget* NoteWidget;

	UPROPERTY(EditInstanceOnly)
	FText NoteText;
};
