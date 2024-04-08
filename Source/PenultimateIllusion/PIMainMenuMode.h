// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PIPBaseUnit.h"
#include "PIMainMenuMode.generated.h"

/**
 * 
 */
UCLASS()
class PENULTIMATEILLUSION_API APIMainMenuMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	//	constructor
	APIMainMenuMode();

	UFUNCTION(BlueprintCallable)
	bool AddSpawnedUnit(UClass* actor);

	UFUNCTION(BlueprintCallable)
	TArray<APIPBaseUnit*> GetSpawnedUnits() const;
	

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//	list of spawned units--
	TArray<APIPBaseUnit*> SpawnedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//	list of "transform" actors
	TArray<AActor*> SpawnTransforms;
};
