// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "BossTower.h"
#include "DA_GameState.h"

void ADA_GameState::StartBossEvent()
{
	if (BossTower)
	{
		BossTower->MoveUp();
		UE_LOG(LogTemp, Warning, TEXT("Moving Tower Up!"));
	}
}

void ADA_GameState::EndBossEvent()
{
	BossTower->MoveDown();
}


