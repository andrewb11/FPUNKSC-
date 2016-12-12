// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CreepCamp.h"
#include "BTTask_CheckCampCleared.h"


EBTNodeResult::Type UBTTask_CheckCampCleared::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACreepCamp* targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));

	if (targetCamp)
	{

		if (targetCamp->GetNumOfCreepsAtCamp() == 0)
		{
			return EBTNodeResult::Succeeded;
		}

		return EBTNodeResult::Failed;

	}

	return EBTNodeResult::Failed;

}

