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

void EnemyCoordinator::RequestToken()
{
	const int32 ATTACK_TOKEN = 1;

	if (AlertedEnemies.Num() == 0)
		return;

	// Check if the token has already been taken
	if (Token == ATTACK_TOKEN) 
		return;

	int32 EnemyToAssign = 0;
	if (AlertedEnemies.Num() > 1)
		EnemyToAssign = FMath::RandHelper(AlertedEnemies.Num());

	Token = ATTACK_TOKEN;
	EnemyWithToken = AlertedEnemies[EnemyToAssign];

	if(IsValid(EnemyWithToken))
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
