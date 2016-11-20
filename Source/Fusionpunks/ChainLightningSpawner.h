// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "ChainLightningSpawner.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AChainLightningSpawner : public AAbilityBase
{
	GENERATED_BODY()
	
protected:	
	virtual bool Ability() override;
private:
	AActor* UpdateTarget();
	UPROPERTY(EditAnywhere, category = Ability)
		TSubclassOf<class AAbilityBase> chainLightningAbility;
};
