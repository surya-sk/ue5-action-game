// Fill out your copyright notice in the Description page of Project Settings.


#include "Progression/TimeJump.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/MainCharacter.h"
#include "Characters/PresentDayCharacter.h"
#include "Progression/MapNames.h"

// Sets default values
ATimeJump::ATimeJump()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimeJump::BeginPlay()
{
	Super::BeginPlay();
}

void ATimeJump::SwitchTimePeriod(EMapName InMapToNavigate, float InTimeDelay, bool bLoadPosition)
{
	MapToNavigate = InMapToNavigate;
	UWorld* World = GEngine->GameViewport->GetWorld(); // DONT SHIP WITH THIS, get a WorldContextObject passed in
	auto* PlayerCharacter = World->GetFirstPlayerController()->GetPawn();
	if (auto* PastCharacter = Cast<AMainCharacter>(PlayerCharacter))
	{
		PastCharacter->SaveGame();
	}
	else if(auto* PresentCharacter = Cast<APresentDayCharacter>(PlayerCharacter))
	{
		PresentCharacter->SaveGame(bLoadPosition);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player character is empty"));
		return;
	}
	PlayerCharacter->GetWorldTimerManager().SetTimer(DelayHandle, this, &ATimeJump::LoadMap, InTimeDelay, false);
}

void ATimeJump::LoadMap()
{
	FName MapName = FMapUtils::GetMapName(MapToNavigate);
	UGameplayStatics::OpenLevel(this, MapName, true);
}

// Called every frame
void ATimeJump::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

