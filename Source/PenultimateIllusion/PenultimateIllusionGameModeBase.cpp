// Copyright Epic Games, Inc. All Rights Reserved.


#include "PenultimateIllusionGameModeBase.h"
#include "PIBattleHud.h"
#include "PIGameInstance.h"
#include "PIAIController.h"
#include "PIPlayerController.h"
#include "PIPBaseUnit.h"


APenultimateIllusionGameModeBase::APenultimateIllusionGameModeBase()
{
	HUDClass = APIBattleHud::StaticClass();
	PlayerControllerClass = APIPlayerController::StaticClass();
	
}

void APenultimateIllusionGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//	attempt to cast to Custom game instance
	auto instance = Cast<UPIGameInstance>(GetWorld()->GetGameInstance());
	check(instance != nullptr && "Game instance was null... wtf");
	//	generate map for traversal if one doesn't exist
	if (instance->LevelMap == nullptr)
	{
		instance->GenerateMap();
	}
	//	assign max difficulty to the current node
	MaxDifficulty = instance->LevelMap->GetCurrentNode()->NodeDifficulty;

	//	generate the AI controller
	enemyController = Cast<APIAIController>(GetWorld()->SpawnActor(BaseAIControllerAsset, &EnemyPartySpawn));
	check(enemyController != nullptr && "EnemyController was null in GameMode, wtf?");
	//	subscribe to OnVictory event
	enemyController->OnVictory.AddDynamic(this, &APenultimateIllusionGameModeBase::HandleVictory);
	
	playerController = Cast<APIPlayerController>(GetWorld()->GetFirstPlayerController());
	check(playerController != nullptr && "PlayerController was null in GameMode, wtf?");

	//	subscribe to OnDefeat event
	playerController->OnDefeat.AddDynamic(this, &APenultimateIllusionGameModeBase::HandleDefeat);
}

void APenultimateIllusionGameModeBase::AddReadyUnit(APIPBaseUnit* unit)
{
	ReadyUnits.AddUnique(unit);

	if (!WaitForAction)
	{
		WaitForAction = true;
	}
	//	check if this is the only element in the list
	if (ReadyUnits.Num() == 1)
	{
		ActingUnitSwap();
	}
}

//	FIFO REMOVAL
void APenultimateIllusionGameModeBase::RemoveUnitAfterAction(APIPBaseUnit* unit)
{
	//	check that size != 0
	if (ReadyUnits.Num() != 0 && ReadyUnits[0] == unit)
	{
		//	remove first entry in list-
		ReadyUnits.RemoveAt(0, 1, true);

		//	check if the size of ready units to 0
		if (ReadyUnits.Num() == 0)
		{
			WaitForAction = false;
		}
		//	if it isn't, assign next unit to wait for action from
		else
		{
			ActingUnitSwap();
		}
	}
}

void APenultimateIllusionGameModeBase::RemoveUnitAfterDeath(APIPBaseUnit* unit)
{
	if (ReadyUnits.Num() != 0)
	{
		ReadyUnits.Remove(unit);

		if (ReadyUnits.Num() != 0)
		{
			if (ActingUnit == unit)
			{
				ActingUnitSwap();
			}
		}
	}
}

void APenultimateIllusionGameModeBase::ActingUnitSwap()
{
	ActingUnit = ReadyUnits[0];
	OnActingUnitChange.Broadcast(ActingUnit);
}

void APenultimateIllusionGameModeBase::HandleDefeat()
{
	GameOver = true;

	APIBattleHud* hud = Cast<APIBattleHud>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hud->SwapHUD();
}

void APenultimateIllusionGameModeBase::HandleVictory()
{
	GameOver = true;
	Victory = true;

	APIBattleHud* hud = Cast<APIBattleHud>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hud->SwapHUD();
}

bool APenultimateIllusionGameModeBase::IsGameOver() const
{
	return GameOver;
}

TArray<AActor*> APenultimateIllusionGameModeBase::GetTransforms() const
{
	return SpawnTransforms;
}

APIAIController* APenultimateIllusionGameModeBase::GetAIController() const
{
	return enemyController;
}

APIPlayerController* APenultimateIllusionGameModeBase::GetPIPlayerController() const
{
	return playerController;
}

