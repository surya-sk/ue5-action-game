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

USTRUCT(BlueprintType)
struct FDialogueData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DialogueLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DialogueAudio;
};

UCLASS()
class UEACTIONGAME_API ADialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueManager();
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE FString GetDialogueLine(EDialogueKey Key) const { return DialogueMap[Key].DialogueLine; }
	FORCEINLINE USoundBase* GetDialogueAudio(EDialogueKey Key) const { return DialogueMap[Key].DialogueAudio; }
	FORCEINLINE FDialogueData GetDialogueData(EDialogueKey Key) const { return DialogueMap[Key]; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EDialogueKey, FDialogueData> DialogueMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
