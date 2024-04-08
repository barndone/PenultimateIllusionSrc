// Fill out your copyright notice in the Description page of Project Settings.


#include "PIBattleHud.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"

void APIBattleHud::SwapHUD()
{
	BattleWidget->SetVisibility(ESlateVisibility::Hidden);
	EndWidget->SetVisibility(ESlateVisibility::Visible);
}

void APIBattleHud::BeginPlay()
{
	Super::BeginPlay();
	UUserWidget* battleWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetHUDClass);
	battleWidget->SetOwningPlayer(GetOwningPlayerController());
	battleWidget->AddToPlayerScreen();

	BattleWidget = battleWidget;
	BattleWidget->SetVisibility(ESlateVisibility::Visible);

	UUserWidget* endWidget = CreateWidget<UUserWidget>(GetWorld(), EndScreenClass);
	endWidget->SetOwningPlayer(GetOwningPlayerController());
	endWidget->AddToPlayerScreen();

	EndWidget = endWidget;
	EndWidget->SetVisibility(ESlateVisibility::Hidden);
}
