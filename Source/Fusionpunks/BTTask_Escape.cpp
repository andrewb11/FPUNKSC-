// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "HeroBase.h"
#include "HeroStats.h"
#include "AbilityBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_Escape.h"

EBTNodeResult::Type UBTTask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr && heroAI != nullptr)
	{		
		healingWell = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("HealingWell"));
		bNotifyTick = true;
		sacrificeCreepAbility = hero->GetAbility(4);
		if (hero->ActorHasTag("Cyber"))
			dashAbility = hero->GetAbility(2);
		else if (hero->ActorHasTag("Diesel"))
			dashAbility = hero->GetAbility(0);
	
		ownedCreepCamps = heroAI->GetSortedOwnedCampList();
		if (ownedCreepCamps.Num() > 0)
		{
			if (hero->GetDistanceTo(ownedCreepCamps[0]) < hero->GetDistanceTo(healingWell) && hero->GetArmySize() == 0)
			{
				destination = ownedCreepCamps[0];
			}

			else
			{
				destination = healingWell;
			}
		}
		else
		{
			destination = healingWell;
		}


		return EBTNodeResult::InProgress;
	}
		return EBTNodeResult::Failed;
}

void UBTTask_Escape::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	movementAbilityDelay += DeltaSeconds;

	if (hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort)
	{

		if (hero->GetArmySize() > 0 && sacrificeCreepAbility != nullptr && sacrificeCreepAbility->CanUse())
		{
			UE_LOG(LogTemp, Error, TEXT("AI Sacrificed Creep"));
			sacrificeCreepAbility->Use();
		}

		else  if (movementAbilityDelay >= 1.25f  && dashAbility!=nullptr && dashAbility->CanUse()  && hero->SafeToJump())
		{
			//OwnerComp.GetAIOwner()->StopMovement();
			dashAbility->Use();
		}


		if (hero->CheckForNearbyOnwedCreepCamps()) 
		{
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}

		else if (hero->CheckForNearbyEnemyHero())
		{
			enemyHero = hero->GetNearbyEnemyHero();

			if (enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() < 0 ||
				enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() < healthPercentageDifference
				|| hero->GetDistanceTo(enemyHero) <= 500)
			{
				UE_LOG(LogTemp, Error, TEXT("AI Thinks It can still fight"));
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}

		


		if (!hero->InsideHealingWell())
		{
			OwnerComp.GetAIOwner()->MoveToActor(destination, 50, true, true, false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MADE IT TO HEALING WELL"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

	else
	{
		heroAI->ResetAITreeTaskStatus();
		//heroAI->ResetAllCampsRecruitStatus();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}





