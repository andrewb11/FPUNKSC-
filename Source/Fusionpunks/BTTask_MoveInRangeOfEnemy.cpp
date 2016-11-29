// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "AbilityBase.h"
#include "HeroBase.h"
#include "Creep.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_MoveInRangeOfEnemy.h"


EBTNodeResult::Type UBTTask_MoveInRangeOfEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr && heroAI !=nullptr)
	{
		heroStats = hero->GetHeroStats();
		target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		ability0 = hero->GetAbility(0);
		ability1 = hero->GetAbility(1);
		ability2 = hero->GetAbility(2);
		ability3 = hero->GetAbility(3);
		if (target != nullptr)
		{
			if (hero->GetDistanceTo(target) >= 300)
			{
				bNotifyTick = true;
				return EBTNodeResult::InProgress;
			}

			return EBTNodeResult::Succeeded;
		}
		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero"));
		return EBTNodeResult::Failed;
	}	

}
void UBTTask_MoveInRangeOfEnemy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(target != NULL)
	{
		if (hero->ActorHasTag("Cyber"))
		{
			if (ability1 != nullptr && ability1->CanUse())
			{
				ability1->Use();
			}

			else if (ability2 != nullptr &&  ability2->CanUse())
			{
				ability2->Use();
			}
		}

		if (hero->GetDistanceTo(target) < 300)
		{
			UE_LOG(LogTemp, Error, TEXT("TOO CLOSE TO TARGET"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}

		else if (hero->CheckForNearbyEnemyHero())
		{
			//ENEMY HERO NEARBY
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}


		else if (heroAI->CheckCampBeingAttacked() && !heroAI->GetCampBeingAttacked()->AIAbondonedCamp())
		{
			UE_LOG(LogTemp, Error, TEXT("Senses Camp Being Attacked"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}

		if (target->IsActorBeingDestroyed())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
		
	else 
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	

}

