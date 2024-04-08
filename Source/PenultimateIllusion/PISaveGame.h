// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PIPBaseUnit.h"
#include "PISaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PENULTIMATEILLUSION_API UPISaveGame : public USaveGame
{
	GENERATED_BODY()
public:



	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TArray<APIPBaseUnit*> Party;

};
