// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCoordinator.h"
#include "Enemy/Enemy.h"

// Init the singleton instance
EnemyCoordinator* EnemyCoordinator::Instance = nullptr;

EnemyCoordinator* EnemyCoordinator::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new EnemyCoordinator();
	}

	return Instance;
}

void EnemyCoordinator::AddEnemy(AEnemy* Enemy)
{
	if (AlertedEnemies.Contains(Enemy) || EnemyWithToken == Enemy)
		return;
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
		return;
	}

	if (Token == ATTACK_TOKEN)
	{
		return;
	}

	// Assign token to a random enemy
	int32 RandomIndex = FMath::RandRange(0, AlertedEnemies.Num() - 1);
	EnemyWithToken = AlertedEnemies[RandomIndex];
	EnemyWithToken->SetTurnToAttack(true);
	Token = ATTACK_TOKEN;
}

void EnemyCoordinator::ReturnToken()
{
	Token = 0;
	EnemyWithToken->SetTurnToAttack(false);
	RequestToken();
}

EnemyCoordinator::EnemyCoordinator()
{
	Token = 0;
}
