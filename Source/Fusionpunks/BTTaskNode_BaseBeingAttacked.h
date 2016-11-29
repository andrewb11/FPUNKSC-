// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_BaseBeingAttacked.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTaskNode_BaseBeingAttacked : public UBTTaskNode
{
	GENERATED_BODY()

protected:		
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	class AHeroBase* hero;
	class AHeroAIController* heroAI;
	TArray<class ATowerBase*> teamTowers;
	
};
