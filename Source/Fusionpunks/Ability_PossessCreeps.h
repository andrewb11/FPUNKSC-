// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "Ability_PossessCreeps.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AAbility_PossessCreeps : public AAbilityBase
{
	GENERATED_BODY()

public:
	AAbility_PossessCreeps();

	virtual void BeginPlay() override;
	virtual bool Ability() override;
	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE float GetCastTimeLeftAsDecimal() const { return castTimer / CastTime; }
	FORCEINLINE float GetChannelTimeLeftAsDecimal() const { return channelTimer / ChannelDuration; }
	FORCEINLINE FName GetAbilityName() const { return AbilityName; }

	bool bIsChanneling;
	bool bIsCasting;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float CastTime;
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float ChannelDuration;
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float AbilityRadius;

	float castTimer;
	float channelTimer;

	class AHeroBase* owner;

	FName AbilityName;

	//UI
	UPROPERTY(EditDefaultsOnly, Category = Widgets)
		TSubclassOf<class UCastBarWidget> CastTimerWidgetClass;
	class UCastBarWidget* castBarWidget;

	AHeroBase* EnemyHero;
	
};
