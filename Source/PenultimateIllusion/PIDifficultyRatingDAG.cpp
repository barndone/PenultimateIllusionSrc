// Fill out your copyright notice in the Description page of Project Settings.


#include "PIDifficultyRatingDAG.h"

PIDifficultyRatingDAG::PIDifficultyRatingDAG()
{
	tiers = 4;
	startingDifficulty = 10;
	maxDifficulty = 50;
}

PIDifficultyRatingDAG::PIDifficultyRatingDAG(int _tiers, int _startingDifficulty, int _maxDifficulty)
{
	tiers = _tiers;
	startingDifficulty = _startingDifficulty;
	maxDifficulty = _maxDifficulty;
}

PIDifficultyRatingDAG::~PIDifficultyRatingDAG()
{
	delete CurrentNode;
	delete EntryNode;
	delete BossNode;
	Nodes.Empty();
}

void PIDifficultyRatingDAG::GenerateGameMap()
{
	//	step 0: determine how many "levels" the graph has
	//	step 1: init root node (entry)
	//	step 1.5: init boss/end node
	//	step 2: determine number of branches (random 1-max)
	//	step 2.5: check which level would be generated- if max-1: point all to end node
	//	step 3: create branches for that node
	//	step 4: repeat steps 2 & 3 until step 2.5 is reached-

	EntryNode = new PIDifficultyRatingNode(startingDifficulty);
	BossNode = new PIDifficultyRatingNode(maxDifficulty);
	
	for (int i = 0; i < tiers; ++i)
	{
		TArray<PIDifficultyRatingNode*> tier;

		if (i == 0)
		{
			tier.Add(EntryNode);
		}

		else if (i == (tiers - 1))
		{
			TArray<PIDifficultyRatingNode*> previousTier = GraphTiers[i - 1];
			for (int j = 0; j < previousTier.Num(); ++j)
			{
				PIDifficultyRatingNode* node = previousTier[j];
				node->ConnectedNodes.Add(BossNode);
			}
			tier.Add(BossNode);
		}

		else
		{
			tier = GenerateTier(GraphTiers[i-1]);
		}
		
		GraphTiers.Add(tier);
	}

	CurrentNode = EntryNode;
}

PIDifficultyRatingNode* PIDifficultyRatingDAG::GetCurrentNode()
{
	return CurrentNode;
}

void PIDifficultyRatingDAG::UpdateCurrentNode(PIDifficultyRatingNode* node)
{
	CurrentNode = node;
}

TArray<PIDifficultyRatingNode*> PIDifficultyRatingDAG::GenerateTier(TArray<PIDifficultyRatingNode*> previousTier)
{
	TArray<PIDifficultyRatingNode*> currentTier{};

	for (int i = 0; i < previousTier.Num(); ++i)
	{
		int branchesToSpawn = rand() % (maxBranch + 1) + 1;
		//	if we aren't the first node
		if (i != 0)
		{
			//	determine if we connect to the last branch of the previous node
			int check = rand() % 2;
			if (check == 1)
			{
				--branchesToSpawn;
				previousTier[i]->ConnectedNodes.Add(previousTier[i-1]->ConnectedNodes.Last());
				for (int j = 0; j < branchesToSpawn; ++j)
				{
					PIDifficultyRatingNode* node = GenerateNode();
					currentTier.AddUnique(node);
					previousTier[i]->ConnectedNodes.Add(node);
				}
			}
			else
			{
				for (int j = 0; j < branchesToSpawn; ++j)
				{
					PIDifficultyRatingNode* node = GenerateNode();
					currentTier.AddUnique(node);
					previousTier[i]->ConnectedNodes.Add(node);
				}
			}
		}

		else
		{
			for (int j = 0; j < branchesToSpawn; ++j)
			{
				PIDifficultyRatingNode* node = GenerateNode();
				currentTier.AddUnique(node);
				previousTier[i]->ConnectedNodes.Add(node);
			}
		}
	}

	return currentTier;
}

PIDifficultyRatingNode* PIDifficultyRatingDAG::GenerateNode()
{
		PIDifficultyRatingNode* newNode = new PIDifficultyRatingNode(GenerateRandomDifficulty());
		return newNode;
}

int PIDifficultyRatingDAG::GenerateRandomDifficulty()
{
	return rand() % maxDifficulty + startingDifficulty;
}
