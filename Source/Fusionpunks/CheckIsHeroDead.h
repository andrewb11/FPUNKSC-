// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CheckIsHeroDead.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UCheckIsHeroDead : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector CurrentTarget;

	class AHeroBase* enemyHero;
	
	
};
