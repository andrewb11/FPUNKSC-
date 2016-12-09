// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "BaseDoor.h"
#include "BTTask_ApproachBase.h"

EBTNodeResult::Type UBTTask_ApproachBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr && heroAI != nullptr)
	{

		enemyBaseDoor  = Cast<ABaseDoor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnemyBaseDoor"));

		if (!enemyBaseDoor->isDestroyed)
			enemyBase = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnemyBaseDoor"));
		else 
			enemyBase = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnemyBaseReactor"));
		if (enemyBase != nullptr)
		{
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
		}

		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}


void UBTTask_ApproachBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if ((hero->GetDistanceTo(enemyBase) > 2500 && hero->CheckForNearbyInteractions()) || (hero->CheckForNearbyEnemyHero() || hero->CheckForNearbyEnemyTowers()))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("FoundNearbyEnemyCamp", hero->GetNearbyEnemyCamp() != nullptr);
		if (hero->GetNearbyEnemyCamp() != nullptr)
		{
			if (heroAI->SafetyCheck(hero->GetNearbyEnemyCamp()))
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("NearbyEnemyCamp", hero->GetNearbyEnemyCamp());
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}


	else
	{
		//UE_LOG(LogTemp, Error, TEXT("Distance From Base Door %f"), hero->GetDistanceTo(enemyBase));
		

		if (!enemyBaseDoor->isDestroyed)
		{
			if (hero->GetDistanceTo(enemyBase) >= 175)
			{
				UE_LOG(LogTemp, Error, TEXT("Distance From Base Door %f "), hero->GetDistanceTo(enemyBase));
				OwnerComp.GetAIOwner()->MoveToActor(enemyBase, 100, false, true, false);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("IN RANGE OF ENEMY BASE"));
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", enemyBase);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}

		else
		{
			if (hero->ActorHasTag("Diesel"))
			{

				if (hero->GetDistanceTo(enemyBase) >= 360)
				{
					UE_LOG(LogTemp, Error, TEXT("Distance From Base  Reactor %f"), hero->GetDistanceTo(enemyBase));
					OwnerComp.GetAIOwner()->MoveToActor(enemyBase, 100, false, true, false);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("IN RANGE OF ENEMY BASE"));
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", enemyBase);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}

			else
			{
				if (hero->GetDistanceTo(enemyBase) >= 300)
				{
					UE_LOG(LogTemp, Error, TEXT("Distance From Base  Reactor %f"), hero->GetDistanceTo(enemyBase));
					OwnerComp.GetAIOwner()->MoveToActor(enemyBase, 100, false, true, false);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("IN RANGE OF ENEMY BASE"));
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", enemyBase);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}
		}
	}
}


