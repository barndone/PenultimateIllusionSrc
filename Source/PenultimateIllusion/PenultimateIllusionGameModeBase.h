// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PenultimateIllusionGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActingUnitChange, class APIPBaseUnit*, unit);

UCLASS()
class PENULTIMATEILLUSION_API APenultimateIllusionGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//	TODO: make all of these fields private
	UPROPERTY(EditAnywhere)
	float BaseChargeTime = 3.0f;
	UPROPERTY(EditAnywhere)
	TArray<APIPBaseUnit*> ReadyUnits;
	UPROPERTY()
	bool WaitForAction = false;
	UPROPERTY()
	class APIPBaseUnit* ActingUnit;

	UPROPERTY(EditAnywhere)
	FVector PlayerPartySpawn;
	UPROPERTY(EditAnywhere)
	FVector EnemyPartySpawn;




	UPROPERTY(EditAnywhere)
	int MaxDifficulty = 3;

	APenultimateIllusionGameModeBase();

	UFUNCTION()
	void AddReadyUnit(APIPBaseUnit* unit);
	UFUNCTION()
	void RemoveUnitAfterAction(APIPBaseUnit* unit);
	UFUNCTION()
		void RemoveUnitAfterDeath(APIPBaseUnit* unit);
	void ActingUnitSwap();

	UFUNCTION()
		void HandleDefeat();

	UFUNCTION()
		void HandleVictory();

	UFUNCTION()
		bool IsGameOver() const;

	UFUNCTION()
		TArray<AActor*> GetTransforms() const;

	class APIAIController* GetAIController() const;
	class APIPlayerController* GetPIPlayerController() const;

	UPROPERTY(BlueprintAssignable)
	FOnActingUnitChange OnActingUnitChange;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UClass* BaseAIControllerAsset;

	UPROPERTY()
	class APIAIController* enemyController;

	UPROPERTY()
	class APIPlayerController* playerController;

	bool GameOver = false;
	bool Victory = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		//	list of "transform" actors
		TArray<AActor*> SpawnTransforms;
};
