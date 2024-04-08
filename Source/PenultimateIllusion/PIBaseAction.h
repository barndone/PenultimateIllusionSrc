// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PIBaseAction.generated.h"

/**
 * 
 */
UCLASS()
class PENULTIMATEILLUSION_API UPIBaseAction : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FText SpellName;
	UPROPERTY(EditAnywhere)
		bool Heal;
	UPROPERTY(EditAnywhere)
		bool Status;
	UPROPERTY(EditAnywhere)
		bool Damage;

	UPROPERTY(EditAnywhere)
		bool TargetEnemies;

	UPROPERTY(EditAnywhere)
		int UnlockLevel = 1;

	UPROPERTY(EditAnywhere)
		bool AoE = false;
};
