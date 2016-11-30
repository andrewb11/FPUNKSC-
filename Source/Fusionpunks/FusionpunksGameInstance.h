// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "HeroBase.h"
#include "FusionpunksGameInstance.generated.h"

/**
 * 
 NOTE::BRENDON - Make 1 class that holds all the enums and structs that we want to use in our project 
 */

UCLASS()
class FUSIONPUNKS_API UFusionpunksGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		ETeam player1Team;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		ETeam player2Team;

	bool bMultiplayer;


	
};
