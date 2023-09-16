// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogueManager.generated.h"

UENUM(BlueprintType)
enum class EDialogueKey
{
	EDK_Echo_Hello UMETA(DisplayName = "Echo Hello"),
	EDK_NPC_Hello UMETA(DisplayName = "NPC hello")
};

UCLASS()
class UEACTIONGAME_API ADialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueManager();
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE FString GetDialogueLine(EDialogueKey Key) const { return DialogueMap[Key]; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EDialogueKey, FString> DialogueMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
