// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "TowerBase.h"
#include "Projectile.h"
#include "ProjectileTowerDamage.h"





void AProjectileTowerDamage::StartTimer(float time, AActor* target)
{

	GetWorld()->GetTimerManager().SetTimer(applyEffectHandle, this, &AProjectileTowerDamage::ApplyEffect, time, true,0);
	effectTarget = target;
}

void AProjectileTowerDamage::ApplyEffect()
{
	UE_LOG(LogTemp, Log, TEXT("TOWER DAMAGE!"));
	FDamageEvent DamageEvent;
	ATowerBase* tower = Cast <ATowerBase> (GetOwner());
	if (effectTarget)
	{
		tower->SpawnProjectiles(effectTarget);
	}
	else
	{
		StopTimer();
	}
		
	//float damage = attackTarget->TakeDamage(20f, DamageEvent, NULL, this);
}


void AProjectileTowerDamage::ChangeTarget(AActor* newTarget)
{
	effectTarget = newTarget;
}

/*
void AProjectileTowerDamage::PauseTimer()
{
	GetWorld()->GetTimerManager().PauseTimer(applyEffectHandle);
}

bool AProjectileTowerDamage::IsPaused()
{
	return GetWorld()->GetTimerManager().IsTimerPaused(applyEffectHandle);
}

void AProjectileTowerDamage::UnPauseTimer(AActor* newTarget)
{
	 effectTarget = newTarget;
	 GetWorld()->GetTimerManager().UnPauseTimer(applyEffectHandle);
}
*/