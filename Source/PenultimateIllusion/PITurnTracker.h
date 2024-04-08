// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PITurnTracker.generated.h"

UCLASS()
class PENULTIMATEILLUSION_API UPITurnTracker : public UUserWidget
{
	GENERATED_BODY()

	//	things turn tracker will need-
		/*
		access to gamemode (readyunits)
		unit panel widget (show portrait, name / health?)
		color panel based on enemy or ally
		*/

		//	on construction- pair all units to a hud element

		//	on ready unit event- update ordering of widgets / hide widgets as needed?

		//	as new units become ready, add them to list from back, as units act pop them from front

public:
	void GenerateUnitPanel(class APIPBaseUnit* unit);

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY()
	class APenultimateIllusionGameModeBase* GameMode;


private:
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> UnitPanel;

	UPROPERTY()
	TArray<UUserWidget*> ReadyUnitPanels;
	
};
