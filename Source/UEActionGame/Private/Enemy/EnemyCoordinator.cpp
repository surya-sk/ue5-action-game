// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCoordinator.h"
#include "Enemy/Enemy.h"

// Init the singleton instance
EnemyCoordinator* EnemyCoordinator::Instance = nullptr;

EnemyCoordinator* EnemyCoordinator::GetInstance()
{
	if (Instance == nullptr)
		Instance = new EnemyCoordinator();

	return Instance;
}

void EnemyCoordinator::AddEnemy(AEnemy* Enemy)
{
	if (AlertedEnemies.Contains(Enemy))
		return;
	AlertedEnemies.Add(Enemy);
}

void EnemyCoordinator::RemoveEnemy(AEnemy* Enemy)
{
	if (!AlertedEnemies.Contains(Enemy))
		return;
	if (Enemy == EnemyWithToken)
		Token = 0;
	AlertedEnemies.Remove(Enemy);
}

void EnemyCoordinator::Reset()
{
	AlertedEnemies.Empty();
}

TArray<AEnemy*> EnemyCoordinator::GetAlertedEnemies() const
{
	return AlertedEnemies;
}

void EnemyCoordinator::RequestToken()
{
	if (Token != 0)
		return;
	int32 EnemyToAssign = 0;
	if (AlertedEnemies.Num() > 1)
		EnemyToAssign = FMath::RandRange(0, AlertedEnemies.Num());

	Token = 1;
	EnemyWithToken = AlertedEnemies[EnemyToAssign];
	EnemyWithToken->SetTurnToAttack(true);
}

void EnemyCoordinator::ReturnToken()
{
	Token = 0;
	EnemyWithToken->SetTurnToAttack(false);
}

EnemyCoordinator::EnemyCoordinator()
{
	Token = 0;
}

EnemyCoordinator::~EnemyCoordinator()
{
}
