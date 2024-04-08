// Fill out your copyright notice in the Description page of Project Settings.


#include "PICheckHealthDecision.h"

PICheckHealthDecision::PICheckHealthDecision()
{
}

PICheckHealthDecision::~PICheckHealthDecision()
{
}

IDecision* PICheckHealthDecision::MakeDecision(APIPBaseUnit* actingUnit, TArray<APIPBaseUnit*>& allies, TArray<APIPBaseUnit*>& enemies)
{
	//	ref for potential target
	APIPBaseUnit* potentialTarget{};

	//	iterate through allies
	for (int i = 0; i < allies.Num(); i++)
	{
		//	check health threshold
		if (allies[i]->GetHealthPercent() <= 0.25f)
		{
			//	assign if null
			if (potentialTarget == nullptr)
			{
				potentialTarget = allies[i];
			}
			//	otherwise
			else
			{
				//	compare health percentages
				if (allies[i]->GetHealthPercent() < potentialTarget->GetHealthPercent())
				{
					potentialTarget = allies[i];
				}
			}
		}
	}

	//	if we have a target, true branch
	if (potentialTarget != nullptr)
	{
		actingUnit->Target = potentialTarget;
		return TrueBranch->MakeDecision(actingUnit, allies, enemies);
	}
	//	else false
	else
	{
		return FalseBranch->MakeDecision(actingUnit, allies, enemies);
	}
}