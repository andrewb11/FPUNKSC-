// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = Player)
		void ResumeGame() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		void ShowOptionsMenu() const;

	
	
};
