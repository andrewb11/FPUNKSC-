// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectOverTimeBase.h"
#include "SlowOverTime.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ASlowOverTime : public AEffectOverTimeBase
{
	GENERATED_BODY()

public:
	ASlowOverTime();
	virtual void StartTimer(float time, AActor* Target) override;
	void SetSnareDuration(float newSnareDuration) { snareDuration = newSnareDuration; }
	void SetSlowPercentage(float newSlowPercentage) { slowPercentage = newSlowPercentage; }
	
protected:
	UFUNCTION()
		virtual	void ApplyEffect() override;

	float snareDuration;
	float slowPercentage;

	void RestoreSpeed();
};
