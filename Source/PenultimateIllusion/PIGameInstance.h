// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PIDifficultyRatingDAG.h"
#include "PIGameInstance.generated.h"

USTRUCT()
struct FUnitData
{
	GENERATED_BODY()
public:
	// stat values
	int Level;
	int MaxHP;
	int CurHP;
	int PhysAtk;
	int PhysDef;
	int MagAtk;
	int MagDef;
	int Spd;
	int Exp;
};

/**
 * 
 */
UCLASS()
//	game instance- things that are available through the lifecycle of the application
//	things more user facing -> game instance
//	 things the (networkded) user shouldn't have acces to -> base gamemode
class PENULTIMATEILLUSION_API UPIGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	PIDifficultyRatingDAG* LevelMap;
	
	UFUNCTION()
		void GenerateMap();

	UFUNCTION()
		TArray<UClass*> GetUnits() const;

	UFUNCTION(BlueprintCallable)
		void AssignParty(TArray<APIPBaseUnit*> party);

	UFUNCTION(BlueprintCallable)
		void SaveStatData(TArray<APIPBaseUnit*> party);

	UFUNCTION(BlueprintCallable)
		void PassStatDataToUnit(TArray<APIPBaseUnit*> party);

private:
	UPROPERTY()
	TArray<UClass*> PlayerParty;
	UPROPERTY()
	TArray<FUnitData> CurrentUnitData;
};
