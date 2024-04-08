// Fill out your copyright notice in the Description page of Project Settings.


#include "PIEndWidget.h"
#include "PIGameInstance.h"
#include "PIBattleSelectButton.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "PIPlayerController.h"

void UPIEndWidget::InitializeLevelSelectButtons()
{
	//	step 1: get current node
	instance = Cast<UPIGameInstance>(GetWorld()->GetGameInstance());
	check(instance != nullptr && "Game instance was null... wtf");
	PIDifficultyRatingNode* current = instance->LevelMap->GetCurrentNode();
	//	step 2: get number of connected nodes
	int connectedNodes = current->ConnectedNodes.Num();
	//	step 3: create button for each connected node
	for (int i = 0; i < connectedNodes; ++i)
	{
		UPIBattleSelectButton* button = NewObject<UPIBattleSelectButton>(UPIBattleSelectButton::StaticClass());
		UTextBlock* title = NewObject<UTextBlock>(UTextBlock::StaticClass());
		int difficulty = current->ConnectedNodes[i]->NodeDifficulty;
		button->SetPairedNode(current->ConnectedNodes[i]);
		//	step 4: pair each button with their corresponding node
		title->SetText(FText::FromString(FString::FromInt(difficulty)));
		//	step 5: pair each button with a function that reloads the level

		button->OnClicked.AddDynamic(button, &UPIBattleSelectButton::HandleClick);
		button->OnLevelSelect.AddDynamic(this, &UPIEndWidget::OnClick);

		ButtonBox->AddChild(button);
		button->AddChild(title);

		buttons.Add(button);
	}
}

void UPIEndWidget::OnClick(UPIBattleSelectButton* passedButton)
{
	instance->LevelMap->UpdateCurrentNode(passedButton->GetPairedNode());

	APIPlayerController* controller = Cast<APIPlayerController>(GetWorld()->GetFirstPlayerController());
	check(controller != nullptr && "PlayerController is null on entering new level, wtf?");
	instance->SaveStatData(controller->GetParty());

	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("TestMap")));
}

void UPIEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeLevelSelectButtons();
}
