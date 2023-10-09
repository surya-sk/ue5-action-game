// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"

/**
 * A Singleton to manage how a group of enemies coordinate to attack the player
 */
class UEACTIONGAME_API EnemyCoordinator
{
public:
	/// <summary>
	/// Gets the singleton instance
	/// </summary>
	/// <returns></returns>
	static EnemyCoordinator* GetInstance();

	/// <summary>
	/// Adds the enemy to the list of alerted enemies
	/// </summary>
	/// <param name="Enemy"></param>
	void AddEnemy(AEnemy* Enemy);

	/// <summary>
	/// Removes the enemy from the list of alerted enemies
	/// </summary>
	/// <param name="Enemy"></param>
	void RemoveEnemy(AEnemy* Enemy);

	/// <summary>
	/// Resets the array, ie, empties it
	/// </summary>
	void Reset();
	
	/// <summary>
	/// Assign token to a random enemy if not taken
	/// </summary>
	void RequestToken();

	/// <summary>
	/// Take the token back from the enemy
	/// </summary>
	void ReturnToken();

private:
	EnemyCoordinator();
	~EnemyCoordinator();

	static EnemyCoordinator* Instance;
	TArray<AEnemy*> AlertedEnemies;
	int32 Token;
	AEnemy* EnemyWithToken;
};
