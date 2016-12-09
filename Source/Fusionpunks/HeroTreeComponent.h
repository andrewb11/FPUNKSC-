// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "HeroTreeComponent.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UHeroTreeComponent : public UBehaviorTreeComponent
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void PauseTree() {PauseLogic("Dead"); }
	FORCEINLINE void ResumeTree() { ResumeLogic("Alive"); }
	FORCEINLINE void AbortTask() { AbortCurrentTask(); }
	
	
};
