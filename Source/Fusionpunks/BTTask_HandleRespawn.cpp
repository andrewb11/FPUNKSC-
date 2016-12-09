// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"	 
#include "HeroAIController.h"
#include "BTTask_HandleRespawn.h"

EBTNodeResult::Type UBTTask_HandleRespawn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	hero = Cast<AHeroBase> (OwnerComp.GetAIOwner()->GetPawn());
	if (hero)
	{
		if (hero->IsRespawning())
		{
			OwnerComp.GetAIOwner()->StopMovement();
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
		}
		
		return EBTNodeResult::Succeeded;

		
	}

	return EBTNodeResult::Failed;


}
void UBTTask_HandleRespawn::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!hero->IsRespawning())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}


}


