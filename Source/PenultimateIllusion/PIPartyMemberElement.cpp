// Fill out your copyright notice in the Description page of Project Settings.


#include "PIPartyMemberElement.h"

void UPIPartyMemberElement::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPIPartyMemberElement::PairNewUnit(APIPBaseUnit* unit)
{
	//	step one- subscribe to events (health update, chagre update)
	Unit->SetText(FText::FromString(unit->UnitName));
	UpdateHealthText(unit->GetCurrentHealth(), unit->GetMaxHealth());
	UpdateChargeMeter(unit->GetChargeValue());

	unit->OnHealthUpdate.AddDynamic(this, &UPIPartyMemberElement::UpdateHealthText);
	unit->OnChargeUpdate.AddDynamic(this, &UPIPartyMemberElement::UpdateChargeMeter);
}

void UPIPartyMemberElement::UpdateHealthText(const int& cur, const int& max)
{
	Health->SetText(FText::FromString("HP: " + FString::FromInt(cur) + " / " + FString::FromInt(max)));
}

void UPIPartyMemberElement::UpdateChargeMeter(const float& fillAmount)
{
	ChargeMeter->SetPercent(fillAmount);
}