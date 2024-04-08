// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PIPBaseUnit.h"

/**
 * 
 */
class PENULTIMATEILLUSION_API IDecision
{
public:
	IDecision();
	~IDecision();

	virtual IDecision* MakeDecision(APIPBaseUnit* actingUnit, TArray<APIPBaseUnit*>& allies, TArray<APIPBaseUnit*>& enemies);
};
