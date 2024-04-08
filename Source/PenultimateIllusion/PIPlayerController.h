// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PIPBaseUnit.h"
#include "PenultimateIllusionGameModeBase.h"
#include "PIPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartyInit, TArray<APIPBaseUnit*>, party);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDefeat);

UCLASS()
class PENULTIMATEILLUSION_API APIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//	set good defaults
	APIPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float _deltaTime) override;
	TArray<APIPBaseUnit*> GetParty();
	bool ContainsUnit(APIPBaseUnit* unit);
	FOnPartyInit OnPartyInit;
	FOnDefeat OnDefeat;

private:
	UPROPERTY(EditAnywhere)
	TArray<APIPBaseUnit*> Party;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int MaxPartyCount = 2;

	UPROPERTY(VisibleAnywhere)
		APIPBaseUnit* ActiveUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UClass* BaseUnitBPAsset;

	UPROPERTY()
		APenultimateIllusionGameModeBase* gameMode;

	UFUNCTION()
		void AssignActiveUnit(APIPBaseUnit* unit);
	UFUNCTION()
	void InitializeParty();
	UFUNCTION()
		void HandlePartyMemberDeath();

	UFUNCTION()
		void InitializeLoadedParty();

	UPROPERTY()
		int DeadUnitCount = 0;
};
