// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "PIPBaseUnit.h"
#include "PIBaseAction.h"
#include "PIButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPIButtonClick, UPIButton*, button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBtnHover, UPIButton*, button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBtnUnHover, UPIButton*, button);



UCLASS()
class PENULTIMATEILLUSION_API UPIButton : public UButton
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	APIPBaseUnit* target;
	UPIBaseAction* action;
	bool TargetButton;
	bool SkillButton;
	UFUNCTION()
		APIPBaseUnit* GetTarget();
	UFUNCTION()
		UPIBaseAction* GetAction();

	UFUNCTION()
		void HandleClick();

	UFUNCTION()
		void HandleHover();
	UFUNCTION()
		void HandleUnHover();

	UPROPERTY(BlueprintCallable)
	FOnPIButtonClick OnPiClicked;
	UPROPERTY(BlueprintCallable)
	FOnBtnHover OnBtnHover;
	UPROPERTY(BlueprintCallable)
	FOnBtnUnHover OnBtnUnHover;
};
