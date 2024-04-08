// Fill out your copyright notice in the Description page of Project Settings.


#include "PIPickAttackDecision.h"

PIPickAttackDecision::PIPickAttackDecision()
{
}

PIPickAttackDecision::~PIPickAttackDecision()
{
}

IDecision* PIPickAttackDecision::MakeDecision(APIPBaseUnit* actingUnit, TArray<APIPBaseUnit*>& allies, TArray<APIPBaseUnit*>& enemies)
{
	UPIBaseDamageSpell* bestAction{};
	int damageToBeat = 0;
	for (int i = 0; i < actingUnit->Actions.Num(); i++)
	{
		UPIBaseDamageSpell* spell = Cast<UPIBaseDamageSpell>(actingUnit->Actions[i]);
		if (spell != nullptr)
		{
			if (spell->SpellData.Physical)
			{
				if ((actingUnit->GetPhysicalAttack() - enemies[0]->GetPhysicalDefense()) > damageToBeat)
				{
					damageToBeat = actingUnit->GetPhysicalAttack() - enemies[0]->GetPhysicalDefense();
					bestAction = spell;
				}
			}
			else
			{
				if ((actingUnit->GetMagicalAttack() - enemies[0]->GetMagicalDefense()) > damageToBeat)
				{
					damageToBeat = actingUnit->GetMagicalAttack() - enemies[0]->GetMagicalDefense();
					bestAction = spell;
				}
			}
		}
	}

	if (bestAction == nullptr)
	{
		actingUnit->CurrentAction = actingUnit->BasicAttack;
	}
	else
	{
		actingUnit->CurrentAction = bestAction;
	}
	actingUnit->InitAttackAnimations();

	return nullptr;
}
