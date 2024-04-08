// Fill out your copyright notice in the Description page of Project Settings.


#include "PIGameInstance.h"
#include "PISaveGame.h"
#include "PIPlayerController.h"
#include <Kismet/GameplayStatics.h>

void UPIGameInstance::GenerateMap()
{
	LevelMap = new PIDifficultyRatingDAG();
	LevelMap->GenerateGameMap();
}

TArray<UClass*> UPIGameInstance::GetUnits() const
{
	return PlayerParty;
}


void UPIGameInstance::AssignParty(TArray<APIPBaseUnit*> party)
{
	PlayerParty.Empty();

	for (int i = 0; i < party.Num(); ++i)
	{
		PlayerParty.Add(party[i]->GetUnitBlueprint());
	}
}

void UPIGameInstance::SaveStatData(TArray<APIPBaseUnit*> party)
{
	CurrentUnitData.Empty();

	for (int i = 0; i < party.Num(); ++i)
	{
		FUnitData data;
		CurrentUnitData.Add(data);

		CurrentUnitData[i].Level = party[i]->GetLevel();
		CurrentUnitData[i].MaxHP = party[i]->GetMaxHealth();
		CurrentUnitData[i].CurHP = party[i]->GetCurrentHealth();
						  
		CurrentUnitData[i].PhysAtk = party[i]->GetPhysicalAttack();
		CurrentUnitData[i].PhysDef = party[i]->GetPhysicalDefense();
						  
		CurrentUnitData[i].MagAtk = party[i]->GetMagicalAttack();
		CurrentUnitData[i].MagDef = party[i]->GetMagicalDefense();
						  
		CurrentUnitData[i].Spd = party[i]->GetSpeed();
						  
		CurrentUnitData[i].Exp = party[i]->GetExp();
	}
}

void UPIGameInstance::PassStatDataToUnit(TArray<APIPBaseUnit*> party)
{
	for (int i = 0; i < party.Num(); ++i)
	{
		party[i]->SetLevel(CurrentUnitData[i].Level);

		party[i]->SetMaxHP(CurrentUnitData[i].MaxHP);
		party[i]->SetCurHP(CurrentUnitData[i].CurHP);

		party[i]->SetPAtk(CurrentUnitData[i].PhysAtk);
		party[i]->SetPDef(CurrentUnitData[i].PhysDef);

		party[i]->SetMAtk(CurrentUnitData[i].MagAtk);
		party[i]->SetMDef(CurrentUnitData[i].MagDef);

		party[i]->SetSpd(CurrentUnitData[i].Spd);

		party[i]->SetEXP(CurrentUnitData[i].Exp);
	}
}



