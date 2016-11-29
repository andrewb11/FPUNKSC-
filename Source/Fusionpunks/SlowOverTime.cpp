// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "SlowOverTime.h"


ASlowOverTime::ASlowOverTime()
{
	
}

void ASlowOverTime::StartTimer(float time, AActor* Target)
{
	UE_LOG(LogTemp, Error, TEXT("Started SlowEnemyCharacter Timer"));
	GetWorld()->GetTimerManager().SetTimer(applyEffectHandle, this, &ASlowOverTime::RestoreSpeed, time, true);
	effectTarget = Target;
	ApplyEffect();
}

void ASlowOverTime::ApplyEffect()
{
	AHeroBase* heroBase = Cast<AHeroBase>(effectTarget);

	if (heroBase)
	{
		heroBase->Slow(1 - slowPercentage);
	}
}

void ASlowOverTime::RestoreSpeed()
{
	if (applyEffectHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(applyEffectHandle);
	}
	AHeroBase* heroBase = Cast<AHeroBase>(effectTarget);

	if (heroBase)
	{
		heroBase->RestoreWalkSpeed();
	}

	effectTarget = nullptr;
	Destroy();
}
