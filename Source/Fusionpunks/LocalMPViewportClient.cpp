// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "DA_GameState.h"
#include "LocalMPViewportClient.h"


bool ULocalMPViewportClient::InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	if (bGamepad && !GetWorld()->GetName().Contains(TEXT("MainMenu")))
	{
		//if in test level check status of if game has started
		if (GetWorld()->GetName().Contains(TEXT("TestLevel")))
		{
			ADA_GameState* gameState = Cast<ADA_GameState>(GetWorld()->GetGameState());
			if (gameState)
			{
				if (gameState->bGameStarted)
				{
					// Map the gamepad to the next player index (so 1st controller is player index 1 (player 2), etc.)
					return Super::InputAxis(Viewport, ControllerId + 1, Key, EventType, AmountDepressed, bGamepad);
				}
			}
		}
		else if (bGamepad && GetWorld()->GetName().Contains(TEXT("GameMap")))
		{
			// Map the gamepad to the next player index (so 1st controller is player index 1 (player 2), etc.)
			return Super::InputAxis(Viewport, ControllerId + 1, Key, EventType, AmountDepressed, bGamepad);
		}
	}
	//player 1 (player controller 0 index) has control over keyboard and gamepad
	return Super::InputKey(Viewport, ControllerId, Key, EventType, AmountDepressed, bGamepad);
}

bool ULocalMPViewportClient::InputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples /*= 1*/, bool bGamepad /*= false*/)
{
	if (bGamepad && !GetWorld()->GetName().Contains(TEXT("MainMenu")))
	{
		//if in test level check status of if game has started
		if (GetWorld()->GetName().Contains(TEXT("TestLevel")))
		{
			ADA_GameState* gameState = Cast<ADA_GameState>(GetWorld()->GetGameState());
			if (gameState)
			{
				if (gameState->bGameStarted)
				{
					// Map the gamepad to the next player index (so 1st controller is player index 1 (player 2), etc.)
					return Super::InputAxis(Viewport, ControllerId + 1, Key, Delta, DeltaTime, NumSamples, bGamepad);
				}
			}
		}
		else if (bGamepad && GetWorld()->GetName().Contains(TEXT("GameMap")))
		{
			// Map the gamepad to the next player index (so 1st controller is player index 1 (player 2), etc.)
			return Super::InputAxis(Viewport, ControllerId + 1, Key, Delta, DeltaTime, NumSamples, bGamepad);
		}
	}
	return Super::InputAxis(Viewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
}

