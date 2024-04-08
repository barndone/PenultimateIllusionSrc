// Fill out your copyright notice in the Description page of Project Settings.


#include "PIAIController.h"
#include "PIPlayerController.h"

APIAIController::APIAIController()
{
	PICanHealDecision* canHealDecision = new PICanHealDecision();
	PICheckHealthDecision* checkAllyHealthDecision = new PICheckHealthDecision();
	PIPickEnemyDecision* pickEnemyTargetDecision = new PIPickEnemyDecision();
	PIPickAttackDecision* pickAttackDecision = new PIPickAttackDecision();

	canHealDecision->TrueBranch = checkAllyHealthDecision;
	canHealDecision->FalseBranch = pickEnemyTargetDecision;

	//	TODO: implement pickHealDecision
	checkAllyHealthDecision->FalseBranch = pickEnemyTargetDecision;

	pickEnemyTargetDecision->TrueBranch = pickAttackDecision;

	//	can heal -> root
	RootDecision = canHealDecision;

	Decisions.Add(canHealDecision);
	Decisions.Add(checkAllyHealthDecision);
	Decisions.Add(pickEnemyTargetDecision);
	Decisions.Add(pickAttackDecision);
}

APIAIController::~APIAIController()
{
	for (int i = 0; i < Decisions.Num(); i++)
	{
		delete Decisions[i];
	}

	Decisions.Empty();
}

void APIAIController::BeginPlay()
{
	Super::BeginPlay();

	APenultimateIllusionGameModeBase* gameMode = Cast<APenultimateIllusionGameModeBase>(GetWorld()->GetAuthGameMode());
	//	if this case did not fail AKA is not null
	check(gameMode != nullptr && "GameMode Was Null on AIController")
	gameMode->OnActingUnitChange.AddDynamic(this, &APIAIController::AssignActiveUnit);

	APIPlayerController* playerController = Cast<APIPlayerController>(GetWorld()->GetFirstPlayerController());
	check(playerController != nullptr && "Playercontroller was null on AIController")
	playerController->OnPartyInit.AddDynamic(this, &APIAIController::PopulatePlayerPartyRef);
	

	GenerateEnemyComp(gameMode->MaxDifficulty);
}

TArray<APIPBaseUnit*> APIAIController::GetParty()
{
	return Party;
}

void APIAIController::GenerateEnemyComp(const int& difficultyRating)
{
	TArray<int> difficultyRatings;
	for(int j = 0; j < PossibleEnemyBP.Num(); ++j)
	{
		APIPBaseUnit* cdoEnemy = Cast< APIPBaseUnit>(PossibleEnemyBP[j]->GetDefaultObject());
		check(cdoEnemy != nullptr && "Invalid entry in PossibleEnemyBP TArray on PIAIController");
		difficultyRatings.Add(cdoEnemy->GetDifficultyRating());
	}

	int currentDifficulty = 0;

	float step = SpawnRange;

	for (int i = 0; (i < difficultyRating) && (currentDifficulty < difficultyRating); ++i)
	{

		//TODO: handle if we can't generate an enemy at a needed rating while the currentDifficulty is not at the max

		if (difficultyRating - currentDifficulty == 0)
		{
			return;
		}

		currentDifficulty += GenerateEnemy(difficultyRating - currentDifficulty, difficultyRatings, step * (i + 1));
	}
}


int APIAIController::GenerateEnemy(const int& maxDifficulty, const TArray<int>& ratings, float xOffset)
{	
	bool validRating = false;
	int index = 0;
	FVector posOffset = FVector(xOffset, 300, 0);
	FRotator rotationOffset = FRotator(FQuat(FVector(0.0, 0.0, 1.0), PI));

	do
	{
		validRating = ValidateDifficultyRating(maxDifficulty, ratings, index);

		bool earlyExit = true;

		for (int i = 0; i < ratings.Num(); ++i)
		{
			if (maxDifficulty >= ratings[i])
			{
				earlyExit = false;
			}
		}
		
		if (earlyExit)
		{
			return 0;
		}

	} while (!validRating);

	//	spawn actor
	APIPBaseUnit* enemy = Cast<APIPBaseUnit>(GetWorld()->SpawnActor(PossibleEnemyBP[index], &posOffset, &rotationOffset));
	check(enemy != nullptr && "null instantiated enemy.... wtf?");
	Party.Add(enemy);
	enemy->IsEnemy = true;
	enemy->OnUnitDeath.AddDynamic(this, &APIAIController::HandlePartyMemberDeath);
	return enemy->GetDifficultyRating();
}

void APIAIController::AssignActiveUnit(APIPBaseUnit* unit)
{
	if (Party.Contains(unit))
	{
		ActiveUnit = unit;

		if (unit->WaitForPriority && unit->WaitingOnPriority)
		{
			//	nothing? idk
		}
		else
		{
			MakeDecision();
		}
	}
	else
	{
		ActiveUnit = nullptr;
	}
}

void APIAIController::MakeDecision()
{
	IDecision* currentDecision = RootDecision;

	do
	{
		currentDecision = currentDecision->MakeDecision(ActiveUnit, Party, PlayerParty);
	} while (currentDecision != nullptr);

	ActiveUnit = nullptr;
}

void APIAIController::PopulatePlayerPartyRef(TArray<APIPBaseUnit*> playerP)
{
	PlayerParty = playerP;
}

void APIAIController::HandlePartyMemberDeath()
{
	++DeadUnitCount;
	if (DeadUnitCount >= Party.Num())
	{
		OnVictory.Broadcast();
	}
}

bool APIAIController::ValidateDifficultyRating(const int& maxDifficulty, const TArray<int>& ratings, int& index) const
{
	int curValue = rand() % (maxDifficulty + 1);

	if (ratings.Contains(curValue))
	{
		auto pos = ratings.Find(curValue);
		index = pos;
		return true;
	}
	else
	{
		return false;
	}
}
