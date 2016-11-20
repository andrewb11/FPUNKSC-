// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "Ability_HammerStorm.h"


void AAbility_HammerStorm::BeginPlay()
{
	Super::BeginPlay();

	rotator = FRotator::FRotator(0, 1, 0);
	rotator *= rotationSpeed;
	owningActor = GetOwner();
}

void AAbility_HammerStorm::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsSpinning)
	{
		if (owningActor)
		{
			owningActor->AddActorLocalRotation(rotator);
		}
	}
}


bool AAbility_HammerStorm::Ability()
{
	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		hero->TurnWithInput(false);
		hero->Slow(1 - movementSpeedPercentage);
	}
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AAbility_HammerStorm::RestoreMovement, abilityDuration, true);
	bIsSpinning = true; 
	return true;
}

void AAbility_HammerStorm::RestoreMovement()
{
	if (timerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	}

	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		hero->TurnWithInput(true);
		hero->RestoreWalkSpeed();
		bIsSpinning = false;
	}
}