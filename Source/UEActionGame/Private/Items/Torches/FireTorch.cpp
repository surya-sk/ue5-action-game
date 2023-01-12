// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Torches/FireTorch.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"

AFireTorch::AFireTorch()
{
	FireParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticles"));
	FireParticles->SetupAttachment(ItemMesh);

	TorchLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("TorchLight"));
	TorchLight->SetupAttachment(ItemMesh);
}

void AFireTorch::BeginPlay()
{
	Super::BeginPlay();

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}
}

void AFireTorch::Unequip()
{
	DetachMeshFromSocket();
	Destroy();
}
