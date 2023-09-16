// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dialogue/DialogueManager.h"
#include "NPC.generated.h"

UCLASS()
class UEACTIONGAME_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

	void Interact();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh");
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	class UTextRenderComponent* DialogueText;

	UPROPERTY(EditInstanceOnly, Category = "Dialogue")
	TArray<EDialogueKey> DialogueKeys;

	UPROPERTY(EditInstanceOnly, Category = "Dialogue")
	ADialogueManager* DialogueManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	int32 CurrentLineIndex;

	void NextLine();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	class AMainCharacter* Player;
	FDialogueData DialogueData;

	void PlayDialogueAudio(USoundBase* DialogueAudio);

};
