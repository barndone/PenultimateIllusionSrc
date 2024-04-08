// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PIPBaseUnit.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "PIPartyMemberElement.generated.h"

/**
 * 
 */
UCLASS()
class PENULTIMATEILLUSION_API UPIPartyMemberElement : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void PairNewUnit(APIPBaseUnit* unit);
	UFUNCTION()
	void UpdateHealthText(const int& cur, const int& max);
	UFUNCTION()
	void UpdateChargeMeter(const float& fillAmount);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BindWidget))
		class UTextBlock* Unit;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BindWidget))
		class UTextBlock* Health;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BindWidget))
		class UProgressBar* ChargeMeter;
};
