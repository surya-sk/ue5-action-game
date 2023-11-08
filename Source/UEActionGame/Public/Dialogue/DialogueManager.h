// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogueManager.generated.h"

UENUM(BlueprintType)
enum class EDialogueKey
{
	/* Test*/
	EDK_Echo_Hello UMETA(DisplayName = "Echo Hello"),
	EDK_NPC_Hello UMETA(DisplayName = "NPC hello"),

	/*Alex and Sarah's conversation*/
	EDK_Alex_Hello UMETA(DisplayName = "Alex: How can I help you?"),
	EDK_Sarah_HaveYouSeenAnythingWeird UMETA(DisplayName = "Sarah: Have you seen anything weird around here? I've woken up to some strange objects around me."),
	EDK_Alex_NotReally UMETA(DisplayName = "Alex: Not really, but guards from the regime often come by to scour for witches. Could've been them."),
	EDK_Sarah_ThatWouldExplainThings UMETA(DisplayName =  "Sarah: Oh that would explain the symbols of the witch hunters."),
	EDK_Alex_ProbablyMarkedAsWitch UMETA(DisplayName = "Alex: Ah, that probably means you've been marked as a witch."),
	EDK_Sarah_IronicCauseImKillingThem UMETA(DisplayName = "Sarah: Ha, ironic considering I'm taking down these bastards for killing innocent women they think are witches."),
	EDK_Alex_BeCareful UMETA(DisplayName = "Alex: Well, I would be very careful if I were you. I heard about a temple near the lake into the woods where one of the leaders operates. Powerful and ruthless man from what I've heard. Has to be stopped if you ask me. Has been killing a lot of folks around this town. ")
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
