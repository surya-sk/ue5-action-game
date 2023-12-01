// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ghost.h"

AGhost::AGhost()
{
}

void AGhost::Tick(float DeltaTime)
{
}

float AGhost::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void AGhost::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
}

void AGhost::BeginPlay()
{
}
