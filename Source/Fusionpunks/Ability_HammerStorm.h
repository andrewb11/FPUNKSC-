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

	bool bIsSpinning = false;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float rotationSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float abilityDuration;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float movementSpeedPercentage;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float timeUntilNextRotation = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float rotateAmount = 1.0f;

	void SpinActor();

	FTimerHandle timerHandle;
	FTimerHandle spinTimerHandle;

	void RestoreMovement();

	AActor* owningActor;

	FRotator rotator; 

	
};
