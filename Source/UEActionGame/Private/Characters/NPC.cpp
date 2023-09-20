// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/MainCharacter.h"
#include "Dialogue/DialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include <AIController.h>
#include <NavigationSystem.h>
#include <NavigationSystem.h>

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMeshComponent* NPCMesh = this->GetMesh();

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(NPCMesh);

	DialogueText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DialogueText"));
	DialogueText->SetupAttachment(NPCMesh);

	CurrentKeyIndex = -1;

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnSphereEndOverlap);

	if (DialogueManager == nullptr)
		UE_LOG(LogTemp, Error, TEXT("Dialogue Manager missing!"));

	if (bShouldPatrol)
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
		MoveToTarget(CurrentPatrolTarget);
	}
}

void ANPC::NextLine()
{
	CurrentKeyIndex++;

	if (CurrentKeyIndex < DialogueKeys.Num())
	{
		bool bPlayerLine = CurrentKeyIndex % 2 != 0;
		DialogueData = DialogueManager->GetDialogueData(DialogueKeys[CurrentKeyIndex]);
		FText DialogueLine = FText::FromString(DialogueData.DialogueLine);
		USoundBase* DialogAudio = DialogueData.DialogueAudio;
		if (bPlayerLine)
		{
			Player->SetDialogueText(DialogueLine);
			Player->PlayDialogueAudio(DialogAudio);
		}
		else
		{
			DialogueText->SetText(DialogueLine);
			PlayDialogueAudio(DialogAudio);
		}
	}
	else
	{
		CurrentKeyIndex = -1;
		DialogueText->SetText(FText::GetEmpty());
	}
}

void ANPC::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AMainCharacter>(OtherActor);
	if (Player)
	{
		Player->SetDialogueState(true, this);
	}
}

void ANPC::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Player)
	{
		Player->SetDialogueState(false);
	}
}

void ANPC::PlayDialogueAudio(USoundBase* DialogueAudio)
{
	if (DialogueAudio)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), DialogueAudio);
		PlayRandomDialogueAnimation();
	}
}

FVector ANPC::ChoosePatrolTarget()
{
	if (PatrolTargets.Num() > 0 && !bShouldPatrol)
	{
		TArray<AActor*> ValidTargets;
		for (auto Target : PatrolTargets)
		{
			if (Target->GetActorLocation() != CurrentPatrolTarget)
			{
				ValidTargets.AddUnique(Target);
			}
		}

		const auto RandomTargetIndex = FMath::RandRange(0, ValidTargets.Num() - 1);
		return ValidTargets[RandomTargetIndex]->GetActorLocation();
	}
	else
	{
		UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

		if (NavigationSystem)
		{
			FNavLocation RandomLocation;
			if (NavigationSystem->GetRandomReachablePointInRadius(GetActorLocation(), ReachablePatrolRadius, RandomLocation))
			{
				return RandomLocation.Location;
			}
		}
		return GetActorLocation();
	}
}

void ANPC::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}

void ANPC::CheckPatrolTarget()
{
	if (IsInTargetRange(CurrentPatrolTarget, PatrolRadius))
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(5.f, 10.f);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &ANPC::PatrolTimerFinished, WaitTime);
	}
}

void ANPC::MoveToTarget(FVector Target)
{
	AAIController* NPCController = Cast<AAIController>(GetController());
	if (NPCController == nullptr)
		return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	NPCController->MoveTo(MoveRequest);
}

bool ANPC::IsInTargetRange(FVector Target, double AcceptanceRadius)
{
	if (Target == FVector::ZeroVector) return false;
	const double DistanceToTarget = (Target - this->GetActorLocation()).Size();
	return DistanceToTarget <= AcceptanceRadius;
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckPatrolTarget();
}

void ANPC::Interact()
{
	if (DialogueKeys.Num() == 0)
		return;

	if (CurrentKeyIndex == -1)
	{
		CurrentKeyIndex = 0;
		DialogueData = DialogueManager->GetDialogueData(DialogueKeys[CurrentKeyIndex]);
		DialogueText->SetText(FText::FromString(DialogueData.DialogueLine));
		PlayDialogueAudio(DialogueData.DialogueAudio);
	}
	else
	{
		NextLine();
	}
}


