// Fill out your copyright notice in the Description page of Project Settings.


#include "PICanHealDecision.h"

PICanHealDecision::PICanHealDecision()
{
}

PICanHealDecision::~PICanHealDecision()
{
}

IDecision* PICanHealDecision::MakeDecision(APIPBaseUnit* actingUnit, TArray<APIPBaseUnit*>& allies, TArray<APIPBaseUnit*>& enemies)
{
	if (actingUnit->CanHeal())
	{
		return TrueBranch->MakeDecision(actingUnit, allies, enemies);
	}
	else
	{
		return FalseBranch->MakeDecision(actingUnit, allies, enemies);
	}

}
