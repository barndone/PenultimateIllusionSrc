// Fill out your copyright notice in the Description page of Project Settings.


#include "PIPickEnemyDecision.h"

PIPickEnemyDecision::PIPickEnemyDecision()
{
}

PIPickEnemyDecision::~PIPickEnemyDecision()
{
}

IDecision* PIPickEnemyDecision::MakeDecision(APIPBaseUnit* actingUnit, TArray<APIPBaseUnit*>& allies, TArray<APIPBaseUnit*>& enemies)
{
	//	arbitrary high value to compare against
	int lowestDefense = 100;

	//	iterate through enemies
	for (int i = 0; i < enemies.Num(); i++)
	{
		//	check physical bulk
		if (actingUnit->GetPhysicalAttack() - enemies[i]->GetPhysicalDefense() < lowestDefense)
		{
			lowestDefense = actingUnit->GetPhysicalAttack() - enemies[i]->GetPhysicalDefense();
			actingUnit->Target = enemies[i];
		}
		//	check magical bulk
		else if (actingUnit->GetMagicalAttack() - enemies[i]->GetMagicalDefense() < lowestDefense)
		{
			lowestDefense = actingUnit->GetMagicalAttack() - enemies[i]->GetMagicalDefense();
			actingUnit->Target = enemies[i];
		}
	}

	//	attack time----
	return TrueBranch->MakeDecision(actingUnit, allies, enemies);
}
