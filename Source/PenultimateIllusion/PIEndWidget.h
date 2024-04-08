// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PIDifficultyRatingNode.h"
#include "Components/HorizontalBox.h"
#include "PIEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class PENULTIMATEILLUSION_API UPIEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void InitializeLevelSelectButtons();
	UFUNCTION()
	void OnClick(UPIBattleSelectButton* passedButton);

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BindWidget))
		class UTextBlock* ResultText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BindWidget))
		class UHorizontalBox* ButtonBox;

	class UPIGameInstance* instance;
	TArray<UPIBattleSelectButton*> buttons;
};
