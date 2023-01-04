// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Torches/FireTorch.h"
#include "Particles/ParticleSystemComponent.h"

AFireTorch::AFireTorch()
{
	FireParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticles"));
	FireParticles->SetupAttachment(ItemMesh);
}

void AFireTorch::Unequip()
{
	DetachMeshFromSocket();
	Destroy();
}
