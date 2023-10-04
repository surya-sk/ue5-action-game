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
	AlertedEnemies.Add(Enemy);
}

void EnemyCoordinator::RemoveEnemy(AEnemy* Enemy)
{
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

EnemyCoordinator::EnemyCoordinator()
{
}

EnemyCoordinator::~EnemyCoordinator()
{
}
