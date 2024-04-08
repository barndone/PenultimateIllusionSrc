// Fill out your copyright notice in the Description page of Project Settings.


#include "PIBattleSelectButton.h"

void UPIBattleSelectButton::SetPairedNode(PIDifficultyRatingNode* node)
{
	PairedNode = node;
}

PIDifficultyRatingNode* UPIBattleSelectButton::GetPairedNode()
{
	return PairedNode;
}

void UPIBattleSelectButton::HandleClick()
{
	OnLevelSelect.Broadcast(this);
}
