// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "RespawnOverTime.h"




void ARespawnOverTime::StartTimer(float time, AActor* target)
{

	GetWorld()->GetTimerManager().SetTimer(applyEffectHandle, this, &ARespawnOverTime::ApplyEffect, time, false);
	effectTarget = target;
	hero = Cast<AHeroBase>(GetOwner());
	if (hero->ActorHasTag("AI"))
		heroAI = Cast<AHeroAIController>(hero->GetController());
}

void ARespawnOverTime::ApplyEffect()
{
	//change animation state machine 
	UBoolProperty* boolProp = FindField<UBoolProperty>(hero->GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsDead"));
	if (boolProp)
	{
		boolProp->SetPropertyValue_InContainer(hero->GetMesh()->GetAnimInstance(), false);
		//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
	}

	hero->SetActorLocation(hero->startingLocation);
	hero->ResetHealth();
	hero->ShowCompassDecal();
	//hero->GetMesh()->SetVisibility(true);
	hero->SetActorEnableCollision(true);
	hero->RestoreWalkSpeed();
	hero->bIsAttacking = false; 
	hero->OnRespawn();
	hero->bHeroDead = false; 

	APlayerController* playerController = Cast<APlayerController>(hero->GetController());
	if (playerController)
	{
		hero->EnableInput(playerController);
	}
	
	hero->bIsRespawning = false;

	if (hero->ActorHasTag("AI"))
	{
		heroAI->ResetAITreeTaskStatus();
	}
}