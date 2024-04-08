// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PIPBaseUnit.h"

#include "CoreMinimal.h"
#include "IDecision.h"
#include "AIController.h"

#include "PICanHealDecision.h"
#include "PICheckHealthDecision.h"
#include "PIPickEnemyDecision.h"
#include "PIPickAttackDecision.h"


#include "PIAIController.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictory);
/**
 * 
 */
UCLASS()
class PENULTIMATEILLUSION_API APIAIController : public AAIController
{
	GENERATED_BODY()

public:
	APIAIController();
	~APIAIController();
protected:
	virtual void BeginPlay() override;

public:
	//	the list of spawned enemies
	UPROPERTY(EditAnywhere)
	TArray<APIPBaseUnit*> Party;

	//	the player Party
	UPROPERTY()
		TArray<APIPBaseUnit*> PlayerParty;

	UPROPERTY()
		float SpawnRange = 200;

	UFUNCTION(BlueprintCallable)
		TArray<APIPBaseUnit*> GetParty();

	FOnVictory OnVictory;

private:
	//	list of possible enemies this controller can spawn per round
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UClass*> PossibleEnemyBP;

	//	the currently active unit (if there is one)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		APIPBaseUnit* ActiveUnit;

	IDecision* RootDecision;

	TArray<IDecision*> Decisions;

	UFUNCTION()
	void GenerateEnemyComp(const int& difficultyRating);
	
	UFUNCTION()
	int GenerateEnemy(const int& maxDifficulty, const TArray<int>& ratings, float xOffset);
	
	UFUNCTION()
	void AssignActiveUnit(APIPBaseUnit* unit);

	UFUNCTION(BlueprintCallable)
		void MakeDecision();

	UFUNCTION()
		void PopulatePlayerPartyRef(TArray<APIPBaseUnit*> playerP);

	UFUNCTION()
		void HandlePartyMemberDeath();

	UPROPERTY()
		int DeadUnitCount = 0;

	bool ValidateDifficultyRating(const int& maxDifficulty, const TArray<int>& ratings, int& index) const;
};
