// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "Ability_Leap.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AAbility_Leap : public AAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual bool Ability() override; 

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float forwardForce;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float upwardsForce;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float gravityScale;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float leapTime;

	float orignalGravityScale;

	void RevertGravityScale();

	FTimerHandle leapTimerHandle;
	
};
