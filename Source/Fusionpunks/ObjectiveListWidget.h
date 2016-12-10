// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ObjectiveListWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UObjectiveListWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = Player)
		ECheckBoxState CheckCaptureObjective();
	UFUNCTION(BlueprintCallable, Category = Player)
		ECheckBoxState CheckRecruitObjective();
	UFUNCTION(BlueprintCallable, Category = Player)
		ECheckBoxState CheckTowerObjective();
	UFUNCTION(BlueprintCallable, Category = Player)
		ECheckBoxState  CheckDoorObjective();
	UFUNCTION(BlueprintCallable, Category = Player)
		ECheckBoxState CheckReactorObjective();

	void SetOwner(class AHeroBase* hero);
protected:

	class AHeroBase* owner;

	bool bCaptured = false, bRecruited = false, bDestroyedTower = false,
		bDestroyedDoor = false, bDestroyedReactor = false;
	
	
	
};
