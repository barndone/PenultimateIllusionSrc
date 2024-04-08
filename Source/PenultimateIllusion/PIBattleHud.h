// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PIBattleHud.generated.h"

/**
 * 
 */
UCLASS()
class PENULTIMATEILLUSION_API APIBattleHud : public AHUD
{
	GENERATED_BODY()
	
public:
	void SwapHUD();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
		TSubclassOf<UUserWidget> WidgetHUDClass;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
		TSubclassOf<UUserWidget> EndScreenClass;


	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess=true))
		UUserWidget* BattleWidget;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UUserWidget* EndWidget;
};
