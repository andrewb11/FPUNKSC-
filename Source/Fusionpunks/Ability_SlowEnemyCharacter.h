// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "Ability_SlowEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AAbility_SlowEnemyCharacter : public AAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual bool Ability() override;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float range;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float SlowPercentage;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float SlowDuration;

	//class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Variables)
	TSubclassOf<class ASlowOverTime> slowOverTimeClass;
	
	
};
