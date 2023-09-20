// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/DialogueManager.h"
#include "BaseCharacter.h"
#include "NPC.generated.h"

UCLASS()
class UEACTIONGAME_API ANPC : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	virtual void Tick(float DeltaTime) override;
	
	void Interact();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue System")
	class UTextRenderComponent* DialogueText;

	UPROPERTY(EditInstanceOnly, Category = "Dialogue System")
	TArray<EDialogueKey> DialogueKeys;

	UPROPERTY(EditInstanceOnly, Category = "Dialogue System")
	ADialogueManager* DialogueManager;

	UPROPERTY(EditInstanceOnly, Category = AI)
	FVector CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = AI)
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = AI)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = AI)
	float ReachablePatrolRadius;

	UPROPERTY(EditAnywhere, Category = AI)
	bool bShouldPatrol = true;

	void NextLine();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	int32 CurrentKeyIndex;

	class AMainCharacter* Player;
	
	FDialogueData DialogueData;

	FTimerHandle PatrolTimer;

	void PlayDialogueAudio(USoundBase* DialogueAudio);

	/// <summary>
	/// Choose a patrol target amoung the targets
	/// </summary>
	/// <returns></returns>
	FVector ChoosePatrolTarget();

	/// <summary>
	/// Wait time for patrol end has finished, move on to the next one
	/// </summary>
	void PatrolTimerFinished();

	/// <summary>
	/// Checks if the current patrol target has been reached, and if true chooses a new one
	/// </summary>
	void CheckPatrolTarget();

	/// <summary>
	/// Moves the enemy to the Target
	/// </summary>
	/// <param name="Target">The actor to move towards</param>
	void MoveToTarget(FVector Target);

	/// <summary>
	/// Checks if the target is within the acceptance radius
	/// </summary>
	/// <param name="Target">The target to check, usually the player</param>
	/// <param name="AcceptanceRadius">The radius to check with</param>
	/// <returns>Whether the target is in range</returns>
	bool IsInTargetRange(FVector Target, double AcceptanceRadius);

};
