// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"
#include "HeroBase.h"
#include "TowerBase.h"
#include "BTTask_CheckEnemyStatus.h"

//This checks the status of the current target
//if the target is pending kill or is dead (i.e. is set to respawn return Fail and set the enemy target to null
EBTNodeResult::Type UBTTask_CheckEnemyStatus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();
	ACreep* owner = Cast<ACreep>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	AActor* enemy = Cast<AActor>(blackboardComponent->GetValueAsObject("EnemyTarget"));
	if (blackboardComponent && owner && enemy)
	{
		//check class
		if (enemy->IsA(ACreep::StaticClass()))
		{
			ACreep* enemyCreep = Cast<ACreep>(enemy);
			if (enemyCreep && !enemyCreep->GetBIsDead())
			{
				return EBTNodeResult::Succeeded;
			}
			else
			{
				ACreepAIController* AiController = Cast<ACreepAIController>(owner->GetController());
				if (AiController)
				{
					AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", nullptr);
				}
				//clear target
				return EBTNodeResult::Failed;
			}
		}
		else if (enemy->IsA(AHeroBase::StaticClass()))
		{
			AHeroBase* enemyHero = Cast<AHeroBase>(enemy);
			if (enemyHero && !enemyHero->bIsRespawning)
			{
				return EBTNodeResult::Succeeded;
			}
			else
			{
				ACreepAIController* AiController = Cast<ACreepAIController>(owner->GetController());
				if (AiController)
				{
					AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", nullptr);
				}
				//clear target
				return EBTNodeResult::Failed;
			}
		}
		else if (enemy->IsA(ATowerBase::StaticClass()))
		{
			if (!enemy->IsPendingKill())
			{
				return EBTNodeResult::Succeeded;
			}
			else
			{
				ACreepAIController* AiController = Cast<ACreepAIController>(owner->GetController());
				if (AiController)
				{
					AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", nullptr);
				}
				//clear target
				return EBTNodeResult::Failed;
			}
		}


		//AHeroBase* enemy = Cast<AHeroBase>(blackboardComponent->GetValueAsObject("EnemyTarget"));
		//ACreep* enemyCreep = Cast<ACreep>(blackboardComponent->GetValueAsObject("EnemyTarget"));
		//if (enemy != nullptr)
		//{
		//	if (!enemy->bIsRespawning)
		//	{
		//		return EBTNodeResult::Succeeded;
		//	}
		//	else
		//	{
		//		ACreepAIController* AiController = Cast<ACreepAIController>(owner->GetController());
		//		if (AiController)
		//		{
		//			AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", nullptr);
		//		}
		//		//clear target
		//		return EBTNodeResult::Failed;
		//	}	
		//}
		//else if (enemyCreep != nullptr && !enemyCreep->GetBIsDead())
		//{
		//	FDamageEvent damageEvent;
		//	enemyCreep->TakeDamage(owner->MeleeAttack(), damageEvent, owner->GetController(), owner);

		//	return EBTNodeResult::Succeeded;
		//}
	}
	blackboardComponent->SetValueAsBool("hasWaited", false);
	return EBTNodeResult::Failed;
}

