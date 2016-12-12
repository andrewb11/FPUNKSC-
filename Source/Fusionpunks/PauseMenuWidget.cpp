// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "FusionpunksGameState.h"
#include "HeroBase.h"
#include "PauseMenuWidget.h"



void UPauseMenuWidget::ResumeGame() const
{
	
	AFusionpunksGameState *gameState;
	gameState = Cast<AFusionpunksGameState>(GetWorld()->GetGameState());
	gameState->UnpauseGame();

}

void UPauseMenuWidget::ShowOptionsMenu() const
{
	/*AFusionpunksGameState *gameState;
	gameState = Cast<AFusionpunksGameState>(GetWorld()->GetGameState());
	gameState->ShowOptionsMenu();
	*/

	Cast<AHeroBase>(GetOwningPlayerPawn())->ShowOptionsMenu();
}