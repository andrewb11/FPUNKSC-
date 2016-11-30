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
	hero->SetActorLocation(hero->startingLocation);
	hero->ResetHealth();
	hero->ShowCompassDecal();
	hero->GetMesh()->SetVisibility(true);
	hero->SetActorEnableCollision(true);
	hero->RestoreWalkSpeed();
	hero->bIsAttacking = false; 
	hero->OnRespawn();

	APlayerController* playerController = Cast<APlayerController>(hero->GetController());
	if (playerController)
	{
		hero->EnableInput(playerController);
	}
	
	hero->bIsRespawning = false;

	if (hero->ActorHasTag("AI"))
	{
		heroAI->ResetAITreeTaskStatus();
		heroAI->RestartHeroAITree();
	}
}