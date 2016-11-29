// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "TowerBase.h"
#include "BTTaskNode_BaseBeingAttacked.h"


EBTNodeResult::Type UBTTaskNode_BaseBeingAttacked::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("GoingForWin"))
		return EBTNodeResult::Failed;


	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (hero != nullptr && heroAI != nullptr)
	{

		teamTowers = hero->GetTeamTowers();

		if (teamTowers.Num() > 0)
		{
			for (int i = 0; i < teamTowers.Num(); i++)
			{
				if (teamTowers[i]->CheckForEnemyHero())
				{
					UE_LOG(LogTemp, Error, TEXT("Found Tower Being Attacked!!!!"));
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("BaseStructure",teamTowers[i]);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("BaseBeingAttacked", true);
					return EBTNodeResult::Succeeded;
				}
			}


			//UE_LOG(LogTemp, Error, TEXT("No Towers Being Attacked!!!!"));
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("BaseBeingAttacked", false);
			return EBTNodeResult::Failed;

		}

		return EBTNodeResult::Failed;

	}

	return EBTNodeResult::Failed;


}

