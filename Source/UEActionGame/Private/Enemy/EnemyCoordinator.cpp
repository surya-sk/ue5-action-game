// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCoordinator.h"
#include "Enemy/Enemy.h"

// Init the singleton instance
EnemyCoordinator* EnemyCoordinator::Instance = nullptr;

EnemyCoordinator* EnemyCoordinator::GetInstance()
{
	if (Instance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating new instance of EnemyCoordinator"));
		Instance = new EnemyCoordinator();
	}

	return Instance;
}

void EnemyCoordinator::AddEnemy(AEnemy* Enemy)
{
	if (AlertedEnemies.Contains(Enemy) || EnemyWithToken == Enemy)
		return;
	UE_LOG(LogTemp, Warning, TEXT("Adding enemy to coordinator"));	
	AlertedEnemies.Add(Enemy);
	RequestToken();
}

void EnemyCoordinator::RemoveEnemy(AEnemy* Enemy)
{
	if (!AlertedEnemies.Contains(Enemy))
		return;
	AlertedEnemies.Remove(Enemy);
	ReturnToken();
}

void EnemyCoordinator::Reset()
{
	AlertedEnemies.Empty();
}

void EnemyCoordinator::RequestToken()
{
	const int32 ATTACK_TOKEN = 1;

	if (AlertedEnemies.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No alerted enemies"));
		return;
	}

	if (Token == ATTACK_TOKEN)
	{
		UE_LOG(LogTemp, Warning, TEXT("Token already assigned"));
		return;
	}

	// Assign token to a random enemy
	int32 RandomIndex = FMath::RandRange(0, AlertedEnemies.Num() - 1);
	EnemyWithToken = AlertedEnemies[RandomIndex];
	EnemyWithToken->SetTurnToAttack(true);
	UE_LOG(LogTemp, Warning, TEXT("Assigning token to enemy"));
	Token = ATTACK_TOKEN;
}

void EnemyCoordinator::ReturnToken()
{
	UE_LOG(LogTemp, Warning, TEXT("Returning token"));
	Token = 0;
	EnemyWithToken->SetTurnToAttack(false);
	RequestToken();
}

EnemyCoordinator::EnemyCoordinator()
{
	Token = 0;
}
