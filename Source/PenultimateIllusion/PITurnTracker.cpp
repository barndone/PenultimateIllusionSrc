// Fill out your copyright notice in the Description page of Project Settings.


#include "PITurnTracker.h"
#include "PIPBaseUnit.h"
#include "PenultimateIllusionGameModeBase.h"

void UPITurnTracker::GenerateUnitPanel(APIPBaseUnit* unit)
{
	//	TODO pair unit to panel
}

void UPITurnTracker::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameMode = Cast<APenultimateIllusionGameModeBase>(GetWorld()->GetAuthGameMode());
	check(GameMode != nullptr && "GameMode ref null on PITurnTracker")
}
