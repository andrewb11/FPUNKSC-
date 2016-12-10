// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckEnemyStatus.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_CheckEnemyStatus : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
