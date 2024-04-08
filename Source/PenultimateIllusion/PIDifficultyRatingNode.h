// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PENULTIMATEILLUSION_API PIDifficultyRatingNode
{
public:
	PIDifficultyRatingNode();
	PIDifficultyRatingNode(int _difficulty);
	~PIDifficultyRatingNode();

	TArray<PIDifficultyRatingNode*> ConnectedNodes;
	int NodeDifficulty = 1;
};
