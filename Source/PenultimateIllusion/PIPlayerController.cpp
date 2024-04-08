// Fill out your copyright notice in the Description page of Project Settings.


#include "PIPlayerController.h"
#include "PIGameInstance.h"

APIPlayerController::APIPlayerController()
{
	
}

void APIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	gameMode = Cast<APenultimateIllusionGameModeBase>(GetWorld()->GetAuthGameMode());
	//	if this case did not fail AKA is not null
	check(gameMode!= nullptr && "GameMode null on PlayerController.... wtf?")
	gameMode->OnActingUnitChange.AddDynamic(this, &APIPlayerController::AssignActiveUnit);

	InitializeParty();
}

void APIPlayerController::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
}

void APIPlayerController::AssignActiveUnit(APIPBaseUnit* unit)
{
	if (Party.Contains(unit))
	{
		ActiveUnit = unit;
	}
	else
	{
		ActiveUnit = nullptr;
	}
}

void APIPlayerController::InitializeParty()
{
	Party.Empty();

	auto instance = Cast<UPIGameInstance>(GetWorld()->GetGameInstance());
	check(instance != nullptr && "Game instance was null... wtf");

	auto partyToSpawn = instance->GetUnits();
	auto positions = gameMode->GetTransforms();


	for (int i = 0; i < partyToSpawn.Num(); ++i)
	{
		APIPBaseUnit* unit = Cast<APIPBaseUnit>(GetWorld()->SpawnActor(partyToSpawn[i], &positions[i]->GetTransform()));
		check(unit != nullptr && "Somehow instantiated null party member...")
			//unit->Rename(unit->UnitName);
		Party.Add(unit);
		unit->OnUnitDeath.AddDynamic(this, &APIPlayerController::HandlePartyMemberDeath);
	}
	
	instance->PassStatDataToUnit(Party);

	OnPartyInit.Broadcast(Party);
}

TArray<APIPBaseUnit*> APIPlayerController::GetParty()
{
	return Party;
}

bool APIPlayerController::ContainsUnit(APIPBaseUnit* unit)
{
	return Party.Contains(unit);
}



void APIPlayerController::HandlePartyMemberDeath()
{
	++DeadUnitCount;
	if (DeadUnitCount >= Party.Num())
	{
		OnDefeat.Broadcast();
	}
}

void APIPlayerController::InitializeLoadedParty()
{

}
