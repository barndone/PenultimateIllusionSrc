// Fill out your copyright notice in the Description page of Project Settings.


#include "PIMainMenuMode.h"

APIMainMenuMode::APIMainMenuMode()
{
	//	not really doing anything, just handling things on blueprint side---
}

bool APIMainMenuMode::AddSpawnedUnit(UClass* actor)
{
	if (SpawnedUnits.Num() < 4)
	{
		//	cache the current spawned unit index for readability
		int curIndex = SpawnedUnits.Num();
		//	store a pointer to the actor we are spawning
		AActor* spawnedActor = GetWorld()->SpawnActor(actor, &SpawnTransforms[curIndex]->GetTransform());

		//	attempt to cast to our bse unit class
		APIPBaseUnit* unit = Cast<APIPBaseUnit>(spawnedActor);

		//	if it isn't null we can add it and return true
		if (unit != nullptr)
		{
			SpawnedUnits.Add(unit);
			return true;
		}

		//	otherwise, destroy the actor we spawned
		else
		{
			spawnedActor->Destroy();
			return false;
		}
	}
	return false;
}

TArray<APIPBaseUnit*> APIMainMenuMode::GetSpawnedUnits() const
{
	return SpawnedUnits;
}

void APIMainMenuMode::BeginPlay()
{
	Super::BeginPlay();
}
