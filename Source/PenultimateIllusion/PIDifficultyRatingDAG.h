// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PIDifficultyRatingNode.h"
#include "CoreMinimal.h"

/**
 * 
 */
class PENULTIMATEILLUSION_API PIDifficultyRatingDAG
{
public:
	PIDifficultyRatingDAG();

	PIDifficultyRatingDAG(int _tiers, int _startingDifficulty, int _maxDifficulty);
	~PIDifficultyRatingDAG();

	void GenerateGameMap();
	PIDifficultyRatingNode* GetCurrentNode();
	void UpdateCurrentNode(PIDifficultyRatingNode* node);


private:
	PIDifficultyRatingNode* CurrentNode;
	PIDifficultyRatingNode* EntryNode;
	PIDifficultyRatingNode* BossNode;
	TArray<PIDifficultyRatingNode*> Nodes;
	TArray<TArray<PIDifficultyRatingNode*>> GraphTiers;

	TArray<PIDifficultyRatingNode*> GenerateTier(TArray<PIDifficultyRatingNode*> previousTier);
	PIDifficultyRatingNode* GenerateNode();
	int GenerateRandomDifficulty();

	int tiers = 4;
	int maxBranch = 3;

	int startingDifficulty = 10;
	int maxDifficulty = 50;
};
