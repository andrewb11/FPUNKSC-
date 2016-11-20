// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "Ability_HammerStorm.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AAbility_HammerStorm : public AAbilityBase
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	virtual bool Ability() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float rotationSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float abilityDuration;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float movementSpeedPercentage;

	bool bIsSpinning = false;

	FTimerHandle timerHandle;

	void RestoreMovement();

	AActor* owningActor;

	FRotator rotator;

	
};
