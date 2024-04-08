// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PIBaseAction.h"
#include "PIBaseDamageSpell.h"
#include "PIBaseHealingSpell.h"
#include "PIPBaseUnit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChargeUpdate, const float&, fillAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdate, const int&, curHealth, const int&,  maxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealSpell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageSpell);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBasicAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTaken);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionFinished);

UCLASS()
class PENULTIMATEILLUSION_API APIPBaseUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APIPBaseUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
		UPIBaseAction* BasicAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
		TArray<UPIBaseAction*> Actions;

	UPROPERTY(BlueprintReadWrite)
	APIPBaseUnit* Target;

	UPROPERTY(BlueprintReadWrite)
	UPIBaseAction* CurrentAction;

	UPROPERTY(BlueprintReadWrite)
	class UBillboardComponent* targetIndicator;


	//	will this unit wait for "priority" to reach this unit?
	UPROPERTY()
		bool WaitForPriority;
	UPROPERTY(BlueprintReadWrite)
		bool WaitingOnPriority;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void InitAttackAnimations();

	void ApplyDamage(const UPIBaseDamageSpell& _ability, APIPBaseUnit* _otherActor);
	void ApplyHealing(const UPIBaseHealingSpell& _ability, APIPBaseUnit* _otherActor);
	UFUNCTION()
	void GainCharge(float DeltaTime);
	UFUNCTION()
	int GetPhysicalAttack();
	UFUNCTION()
	int GetMagicalAttack();

	UFUNCTION()
		int GetPhysicalDefense();
	UFUNCTION()
		int GetMagicalDefense();

	UFUNCTION()
	int GetCurrentHealth();
	UFUNCTION()
	int GetMaxHealth();
	UFUNCTION()
	float GetChargeValue();
	UFUNCTION()
		bool CanHeal();
	UFUNCTION()
		float GetHealthPercent();

	UFUNCTION()
		void NormalAttack();
	UFUNCTION(BlueprintCallable)
		void TakeAction();
	UFUNCTION()
		bool IsDead() const;

	UFUNCTION()
		int GetLevel() const;
	UFUNCTION()
		int GetSpeed() const;

	UFUNCTION()
		int GetExp() const;

	UFUNCTION()
	void SetLevel(const int& value);
	UFUNCTION()
	void SetMaxHP(const int& value);
	UFUNCTION()
	void SetCurHP(const int& value);
	UFUNCTION()
	void SetPAtk(const int& value);
	UFUNCTION()
	void SetPDef(const int& value);
	UFUNCTION()
	void SetMAtk(const int& value);
	UFUNCTION()
	void SetMDef(const int& value);
	UFUNCTION()
	void SetSpd(const int& value);
	UFUNCTION()
	void SetEXP(const int& value);


	UFUNCTION()
		int GetDifficultyRating() const;

	UFUNCTION()
		UClass* GetUnitBlueprint();
	UFUNCTION()
		void SetUnitBlueprint(UClass* value);

	FOnHealthUpdate OnHealthUpdate;
	FOnChargeUpdate OnChargeUpdate;
	UPROPERTY(BlueprintAssignable)
	FOnUnitDeath OnUnitDeath;
	UPROPERTY(BlueprintAssignable)
	FOnHealSpell OnHealSpell;
	UPROPERTY(BlueprintAssignable)
	FOnDamageSpell OnDamageSpell;
	UPROPERTY(BlueprintAssignable)
	FOnBasicAttack OnBasicAttack;
	UPROPERTY(BlueprintAssignable)
	FOnActionFinished OnActionFinished;
	UPROPERTY(BlueprintAssignable)
	FOnDamageTaken OnDamageTaken;

	UFUNCTION(BlueprintCallable)
		void ToggleTargetingIndicator(bool state);

	UPROPERTY()
	bool IsEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
		FString UnitName;


protected:
	//	the level of this unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int Level = 1;
	//	the max health of this unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int MaxHealth = 10;
	//	 the current health of this unit (Initialized to maxHealth in constructor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int CurrentHealth;
	//	the physical attack of this unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int PhysicalAttack = 5;
	//	 the physical defense of this unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int PhysicalDefense = 5;
	//	 the magical attack of this unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int MagicalAttack = 5;
	//	 the magical defense of this unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int MagicalDefense = 5;
	//	 the speed of this unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int Speed = 5;
	//	the experience this unit has accumulated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int Experience = 0;
	//	the experience this unit is worth
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int ExperienceVal = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	float AccumulatedTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	float ChargeTime = 0.0f;
	bool CanAct = false;
	bool IsHealer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = true))
	int DifficultyValue = 1;
	
	UPROPERTY()
	class APenultimateIllusionGameModeBase* gameMode;

	void CalculateChargeMultiplier();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UClass* BaseUnitBPAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		class UImage* UnitIcons;

};
