// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Escape.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_Escape : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	class AHeroBase *hero, *enemyHero;
	class AHeroAIController* heroAI;
	AActor* healingWell;
	class AAbilityBase* sacrificeCreepAbility, *dashAbility;
	TArray<ACreepCamp*> ownedCreepCamps;
	AActor* destination;
	UPROPERTY(EditAnywhere)
		float healthPercentageAbort = 0.21f;
	UPROPERTY(EditAnywhere)
		float healthPercentageDifference = 0.35f;
	float movementAbilityDelay = 0;
	
	
	
	
	
};
