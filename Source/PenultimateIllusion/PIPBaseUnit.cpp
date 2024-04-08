// Fill out your copyright notice in the Description page of Project Settings.


#include "PIPBaseUnit.h"
#include "PIAIController.h"
#include "PIPlayerController.h"
#include "Components/BillboardComponent.h"
#include "PenultimateIllusionGameModeBase.h"

// Sets default values
APIPBaseUnit::APIPBaseUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APIPBaseUnit::BeginPlay()
{
	Super::BeginPlay();


	//	TODO: Check whether we are in the main menu or not- so that we can spawn them then? or not? 
	gameMode = Cast<APenultimateIllusionGameModeBase>(GetWorld()->GetAuthGameMode());
	//	if this case did not fail AKA is not null
	//check(gameMode != nullptr && "GameMode null on BaseUnit")

	this->CurrentHealth = this->MaxHealth;

	if (gameMode != nullptr)
	{
		ChargeTime = gameMode->BaseChargeTime;
		CalculateChargeMultiplier();
		//	otherwise do nothing
	}
}

// Called every frame
void APIPBaseUnit::Tick(float DeltaTime)
{
	if (gameMode != nullptr)
	{
		if (!IsDead() && !gameMode->IsGameOver())
		{
			Super::Tick(DeltaTime);
			GainCharge(DeltaTime);
		}
	}
}

void APIPBaseUnit::InitAttackAnimations()
{
	//	first cast to damage
	UPIBaseDamageSpell* attack = Cast<UPIBaseDamageSpell>(CurrentAction);

	if (attack != nullptr)
	{
		if (attack->SpellData.Physical)
		{
			OnBasicAttack.Broadcast();
		}
		else
		{
			OnDamageSpell.Broadcast();
		}
	}
	else
	{
		UPIBaseHealingSpell* heal = Cast<UPIBaseHealingSpell>(CurrentAction);
		if (heal != nullptr)
		{
			OnHealSpell.Broadcast();
		}
	}

	if (Target->WaitForPriority)
	{
		Target->WaitingOnPriority = true;
	}
}

void APIPBaseUnit::NormalAttack()
{
	CurrentAction = BasicAttack;
}

void APIPBaseUnit::TakeAction()
{
	//	first cast to damage
	UPIBaseDamageSpell* attack = Cast<UPIBaseDamageSpell>(CurrentAction);

	if (attack != nullptr)
	{
		if (!attack->AoE)
		{
			//	do the attack
			Target->ApplyDamage(*attack, this);
			Target->ToggleTargetingIndicator(false);
		}
		else
		{
			if (Target->IsEnemy)
			{
				auto enemyParty = gameMode->GetAIController()->GetParty();
				for (int i = 0; i < enemyParty.Num(); ++i)
				{
					enemyParty[i]->ApplyDamage(*attack, this);
				}
			}
			else
			{
				auto playerParty = gameMode->GetPIPlayerController()->GetParty();
				for (int i = 0; i < playerParty.Num(); ++i)
				{
					playerParty[i]->ApplyDamage(*attack, this);
				}
			}
		}

	}
	else
	{
		UPIBaseHealingSpell* heal = Cast<UPIBaseHealingSpell>(CurrentAction);
		if (heal != nullptr)
		{
			if (!heal->AoE)
			{
				//do the heal
				Target->ApplyHealing(*heal, this);
				Target->ToggleTargetingIndicator(false);
			}
			else
			{
				if (Target->IsEnemy)
				{
					auto enemyParty = gameMode->GetAIController()->GetParty();
					for (int i = 0; i < enemyParty.Num(); ++i)
					{
						enemyParty[i]->ApplyHealing(*heal, this);
					}
				}
				else
				{
					auto playerParty = gameMode->GetPIPlayerController()->GetParty();
					for (int i = 0; i < playerParty.Num(); ++i)
					{
						playerParty[i]->ApplyHealing(*heal, this);
					}
				}
			}
		}
	}


	CanAct = false;
	AccumulatedTime = 0.0f;
	Target = nullptr;
	CurrentAction = nullptr;
	gameMode->RemoveUnitAfterAction(this);
}

void APIPBaseUnit::ApplyDamage(const UPIBaseDamageSpell& _ability, APIPBaseUnit* _otherActor)
{
	int damageToTake = 0;

	//	if this is a physical ability/spell
	if (_ability.SpellData.Physical)
	{
		//	calculate using physical stats
		int baseDamage = (_otherActor->GetPhysicalAttack() / 2) - (PhysicalDefense / 2);
		if (baseDamage < 0) { baseDamage = 0; }
		float multipliedDamage = (float)baseDamage * _ability.SpellData.DamageMultiplier;
		damageToTake = (int)multipliedDamage;
	}
	//	otherwise, it is magical
	else
	{
		//	calculate using magical defense
		int baseDamage = (_otherActor->GetMagicalAttack() / 2) - (MagicalDefense / 2);
		if (baseDamage < 0) { baseDamage = 0; }
		float multipliedDamage = (float)baseDamage * _ability.SpellData.DamageMultiplier;
		damageToTake = (int)multipliedDamage;
	}

	CurrentHealth -= damageToTake;

	if (CurrentHealth <= 0)
	{
		OnUnitDeath.Broadcast();
		gameMode->RemoveUnitAfterDeath(this);
	}

	OnDamageTaken.Broadcast();

	OnHealthUpdate.Broadcast(CurrentHealth, MaxHealth);
}

void APIPBaseUnit::ApplyHealing(const UPIBaseHealingSpell& _ability, APIPBaseUnit* _otherActor)
{
	int healingToApply = 0;

	int baseHealing = _otherActor->GetMagicalAttack() / 2;
	float multipliedHealing = (float)baseHealing * _ability.SpellData.MagicMultiplier;
	healingToApply = (int)multipliedHealing;

	CurrentHealth += healingToApply;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	OnHealthUpdate.Broadcast(CurrentHealth, MaxHealth);
}

void APIPBaseUnit::GainCharge(float DeltaTime)
{
	if (AccumulatedTime >= ChargeTime)
	{
		if (!CanAct)
		{
			OnChargeUpdate.Broadcast(ChargeTime / ChargeTime);
			CanAct = true;

			gameMode->AddReadyUnit(this);

			//	TODO: add event to gamemode that broadcasts when a unit is added to the ready units list (to construct a panel for the turnTracker)
		}
	}
	else
	{
		//	TODO: check if the player is in a menu before incrementing time
		AccumulatedTime += DeltaTime;

		OnChargeUpdate.Broadcast(AccumulatedTime / ChargeTime);
	}
}

int APIPBaseUnit::GetPhysicalAttack()
{
	return PhysicalAttack;
}

int APIPBaseUnit::GetMagicalAttack()
{
	return MagicalAttack;
}

int APIPBaseUnit::GetPhysicalDefense()
{
	return PhysicalDefense;
}

int APIPBaseUnit::GetMagicalDefense()
{
	return MagicalDefense;
}

void APIPBaseUnit::CalculateChargeMultiplier()
{
	int speedBreakpoints = Speed / 2;
	float chargeMultiplier = speedBreakpoints * 0.05f;

	ChargeTime = ChargeTime - chargeMultiplier;
}

bool APIPBaseUnit::IsDead() const
{
	return CurrentHealth <= 0;
}

int APIPBaseUnit::GetLevel() const
{
	return Level;
}

int APIPBaseUnit::GetSpeed() const
{
	return Speed;
}

int APIPBaseUnit::GetExp() const
{
	return Experience;
}

void APIPBaseUnit::SetLevel(const int& value)
{
	Level = value;
}

void APIPBaseUnit::SetMaxHP(const int& value)
{
	MaxHealth = value;
}

void APIPBaseUnit::SetCurHP(const int& value)
{
	CurrentHealth = value;
}

void APIPBaseUnit::SetPAtk(const int& value)
{
	PhysicalAttack = value;
}

void APIPBaseUnit::SetPDef(const int& value)
{
	PhysicalDefense = value;
}

void APIPBaseUnit::SetMAtk(const int& value)
{
	MagicalAttack = value;
}

void APIPBaseUnit::SetMDef(const int& value)
{
	MagicalDefense = value;
}

void APIPBaseUnit::SetSpd(const int& value)
{
	Speed = value;
}

void APIPBaseUnit::SetEXP(const int& value)
{
	Experience = value;
}

int APIPBaseUnit::GetDifficultyRating() const
{
	return DifficultyValue;
}

UClass* APIPBaseUnit::GetUnitBlueprint()
{
	return BaseUnitBPAsset;
}

void APIPBaseUnit::SetUnitBlueprint(UClass* value)
{
	BaseUnitBPAsset = value;
}

void APIPBaseUnit::ToggleTargetingIndicator(bool state)
{
	//	TODO: get target indicator working properly
	// does not hide / show based off of whether the unit is targeted or not
	//targetIndicator->SetActive(state);
}

int APIPBaseUnit::GetCurrentHealth()
{
	return CurrentHealth;
}

int APIPBaseUnit::GetMaxHealth()
{
	return MaxHealth;
}

float APIPBaseUnit::GetChargeValue()
{
	return AccumulatedTime / ChargeTime;
}

bool APIPBaseUnit::CanHeal()
{
	return IsHealer;
}

float APIPBaseUnit::GetHealthPercent()
{
	return (float)(CurrentHealth / MaxHealth);
}
