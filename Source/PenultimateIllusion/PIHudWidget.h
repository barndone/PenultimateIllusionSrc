// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PIPBaseUnit.h"
#include "PIButton.h"
#include "PenultimateIllusionGameModeBase.h"
#include "PIHudWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClick, APIPBaseUnit*, target);

UCLASS()
class PENULTIMATEILLUSION_API UPIHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void HandleNewActingUnit(APIPBaseUnit* unit);
	UFUNCTION()
	void InitializeAvailableSkills();
	UFUNCTION()
	void InitializePartyHud(TArray<APIPBaseUnit*> partyToInit);
	UFUNCTION()
	void InitializeTargetingButtons(const UPIBaseAction* action);

	UPROPERTY()
	FOnClick Click;


protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BindWidget))
	class UScrollBox* ParentObject;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true, BindWidget))
	class UVerticalBox* PartyStatsHud;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class UButton*> InitializedButtons;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class APIPBaseUnit* currentUnit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> PartyStatWidget;

	UPROPERTY()
	class APIPlayerController* controller;

	UPROPERTY()
	 class APenultimateIllusionGameModeBase* gameMode;

	UPROPERTY()
	 class APIAIController* aiController;

	UPROPERTY()
	 class UPIBaseAction* currentAction;

	void CleanUpButtons();
	UFUNCTION()
		void GetBasicAttackTargets();
	UFUNCTION()
		void OnClick(UPIButton* button);

	UFUNCTION()
		void OnHover(UPIButton* button);
	UFUNCTION()
		void OnUnhover(UPIButton* button);
};
