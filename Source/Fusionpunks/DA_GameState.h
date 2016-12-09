// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "DA_GameState.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ADA_GameState : public AGameState
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(BlueprintReadWrite, Category = DA_GameState)
	bool bGameStarted;

	
};
