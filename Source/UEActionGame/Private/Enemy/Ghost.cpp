// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ghost.h"

AGhost::AGhost()
{
}

void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AGhost::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AGhost::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Hitter);
}

void AGhost::BeginPlay()
{
	Super::BeginPlay();

	if (Action == EGhostAction::EGA_AppearAndDisappear)
	{
		GetWorldTimerManager().SetTimer(DestroyTimer, this, &AGhost::Disappear, DisappearIn, false);
	}
	else if (Action == EGhostAction::EGA_WalkBy)
	{

	}
}

void AGhost::Die()
{
	Super::Die();
}

void AGhost::Attack()
{
}

bool AGhost::CanAttack()
{
	return Super::CanAttack();
}

void AGhost::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
}

void AGhost::AttackEnd()
{
}

void AGhost::Disappear()
{
	Destroy();
}
