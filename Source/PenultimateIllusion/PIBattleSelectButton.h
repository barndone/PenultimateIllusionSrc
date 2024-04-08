// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "PIDifficultyRatingNode.h"

#include "PIBattleSelectButton.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelSelect, UPIBattleSelectButton*, button);
/**
 * 
 */
UCLASS()
class PENULTIMATEILLUSION_API UPIBattleSelectButton : public UButton
{
	GENERATED_BODY()

public:
	
	void SetPairedNode(PIDifficultyRatingNode* node);
	class PIDifficultyRatingNode* GetPairedNode();
	UFUNCTION()
	void HandleClick();
	UPROPERTY()
	FOnLevelSelect OnLevelSelect;
	
private:
	PIDifficultyRatingNode* PairedNode;


};
