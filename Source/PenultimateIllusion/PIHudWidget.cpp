// Fill out your copyright notice in the Description page of Project Settings.


#include "PIHudWidget.h"
#include "PenultimateIllusionGameModeBase.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "PIPartyMemberElement.h"
#include "PIPlayerController.h"
#include "PIAIController.h"
#include "Components/VerticalBox.h"

void UPIHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gameMode = Cast<APenultimateIllusionGameModeBase>(GetWorld()->GetAuthGameMode());
	//	if this case did not fail AKA is not null
	if (gameMode != nullptr)
	{
		gameMode->OnActingUnitChange.AddDynamic(this, &UPIHudWidget::HandleNewActingUnit);
	}

	controller = Cast<APIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (controller != nullptr)
	{
		controller->OnPartyInit.AddDynamic(this, &UPIHudWidget::InitializePartyHud);
	}

	aiController = Cast<APIAIController>(gameMode->GetAIController());
	if (aiController != nullptr)
	{

	}

	controller->OnPartyInit.Broadcast(controller->GetParty());
}

void UPIHudWidget::CleanUpButtons()
{
	for (int i = 0; i < InitializedButtons.Num(); ++i)
	{
		InitializedButtons[i]->ConditionalBeginDestroy();
		ParentObject->RemoveChild(InitializedButtons[i]);
	}

	InitializedButtons.Empty();
}


void UPIHudWidget::GetBasicAttackTargets()
{
	currentAction = currentUnit->BasicAttack;
	currentUnit->CurrentAction = currentAction;
	InitializeTargetingButtons(currentUnit->BasicAttack);
}

void UPIHudWidget::OnClick(UPIButton* button)
{
	if (button->TargetButton)
	{
		currentUnit->Target = button->GetTarget();
		currentUnit->InitAttackAnimations();
	}
	else
	{
		currentAction = button->GetAction();
		currentUnit->CurrentAction = currentAction;

		if (currentAction->AoE)
		{
			if (currentAction->TargetEnemies)
			{
				currentUnit->Target = gameMode->GetAIController()->Party[0];
			}
			else
			{
				currentUnit->Target = currentUnit;
			}
			currentUnit->InitAttackAnimations();
		}
		else
		{
			InitializeTargetingButtons(currentAction);
		}
	}
}

void UPIHudWidget::OnHover(UPIButton* button)
{
	button->target->ToggleTargetingIndicator(true);
}

void UPIHudWidget::OnUnhover(UPIButton* button)
{
	button->target->ToggleTargetingIndicator(false);
}

void UPIHudWidget::HandleNewActingUnit(APIPBaseUnit* unit)
{
	CleanUpButtons();
	currentAction = nullptr;

	if (unit->IsDead())
	{
		gameMode->RemoveUnitAfterDeath(unit);
		return;
	}

	if (controller->ContainsUnit(unit))
	{
		currentUnit = unit;
		UPIButton* attack = NewObject<UPIButton>(UPIButton::StaticClass(), TEXT("attack"));
		UTextBlock* attackTitle = NewObject<UTextBlock>(UTextBlock::StaticClass(), TEXT("Title"));
		attackTitle->SetText(FText::FromString("Attack"));
		attack->AddChild(attackTitle);
		ParentObject->AddChild(attack);
		InitializedButtons.Add(attack);

		UPIButton* skill = NewObject<UPIButton>(UPIButton::StaticClass(), TEXT("skill"));
		UTextBlock* skillTitle = NewObject<UTextBlock>(UTextBlock::StaticClass(), TEXT("Title"));
		skillTitle->SetText(FText::FromString("Skills"));
		skill->AddChild(skillTitle);
		ParentObject->AddChild(skill);
		InitializedButtons.Add(skill);

		//	TODO: bind attack OnClick to show available targets-- show estimated damage
		attack->OnClicked.AddDynamic(this, &UPIHudWidget::GetBasicAttackTargets);
		//	TODO: check if unit is taunted, if so, skip to just attacking the target
		skill->OnClicked.AddDynamic(this, &UPIHudWidget::InitializeAvailableSkills);
	}
}

void UPIHudWidget::InitializeAvailableSkills()
{
	CleanUpButtons();

	for (int i = 0; i < currentUnit->Actions.Num(); i++)
	{
		if ( currentUnit->GetLevel() >= currentUnit->Actions[i]->UnlockLevel )
		{
			UPIButton* button = NewObject<UPIButton>(UPIButton::StaticClass());
			UTextBlock* title = NewObject<UTextBlock>(UTextBlock::StaticClass());
			ParentObject->AddChild(button);

			button->SkillButton = true;
			button->action = currentUnit->Actions[i];

			button->OnClicked.AddDynamic(button, &UPIButton::HandleClick);
			button->OnPiClicked.AddDynamic(this, &UPIHudWidget::OnClick);

			title->SetText(currentUnit->Actions[i]->SpellName);
			button->AddChild(title);
			InitializedButtons.AddUnique(button);
		}
	}
}

void UPIHudWidget::InitializePartyHud(TArray<APIPBaseUnit*> partyToInit)
{
	for (int i = 0; i < partyToInit.Num(); i++)
	{
		UPIPartyMemberElement* partyBar = Cast<UPIPartyMemberElement>(CreateWidget<UUserWidget>(GetWorld(), PartyStatWidget));
		if (partyBar != nullptr)
		{
			PartyStatsHud->AddChild(partyBar);
			partyBar->PairNewUnit(partyToInit[i]);
		}
	}
}

void UPIHudWidget::InitializeTargetingButtons(const UPIBaseAction* action)
{
	//TODO: fix lock if no valid target
	CleanUpButtons();

	//	branch 1: target enemies
	if (action->TargetEnemies)
	{
		for (int i = 0; i < gameMode->GetAIController()->Party.Num(); ++i)
		{
			auto unit = gameMode->GetAIController()->Party[i];
			if (!unit->IsDead())
			{
				UPIButton* button = NewObject<UPIButton>(UPIButton::StaticClass());
				UTextBlock* title = NewObject<UTextBlock>(UTextBlock::StaticClass());
				ParentObject->AddChild(button);

				title->SetText(FText::FromString(unit->UnitName));
				button->AddChild(title);
				InitializedButtons.Add(button);

				button->TargetButton = true;
				button->target = unit;
				button->OnClicked.AddDynamic(button, &UPIButton::HandleClick);
				button->OnPiClicked.AddDynamic(this, &UPIHudWidget::OnClick);

				button->OnHovered.AddDynamic(button, &UPIButton::HandleHover);
				button->OnBtnHover.AddDynamic(this, &UPIHudWidget::OnHover);
				button->OnUnhovered.AddDynamic(button, &UPIButton::HandleUnHover);
				button->OnBtnUnHover.AddDynamic(this, &UPIHudWidget::OnUnhover);
			}

		}
	}
	//	branch 2: target allies
	else 
	{
		for (int i = 0; i < controller->GetParty().Num(); ++i)
		{
			auto* unit = controller->GetParty()[i];
			if (!unit->IsDead())
			{
				UPIButton* button = NewObject<UPIButton>(UPIButton::StaticClass());
				UTextBlock* title = NewObject<UTextBlock>(UTextBlock::StaticClass());
				ParentObject->AddChild(button);
		
				title->SetText(FText::FromString(unit->UnitName));
				button->AddChild(title);
				InitializedButtons.Add(button);

				button->TargetButton = true;	
				button->target = unit;
				button->OnClicked.AddDynamic(button, &UPIButton::HandleClick);
				button->OnPiClicked.AddDynamic(this, &UPIHudWidget::OnClick);
			}
		}
	}
}
