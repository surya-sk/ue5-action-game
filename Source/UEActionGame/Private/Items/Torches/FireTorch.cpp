// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Torches/FireTorch.h"

void AFireTorch::Unequip()
{
	DetachMeshFromSocket();
	Destroy();
}
