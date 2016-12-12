// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "HeroAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HeroBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "HeroStats.h"
#include "AbilityBase.h"
#include "BTTask_MoveToCamp.h"

EBTNodeResult::Type UBTTask_MoveToCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	neutralCampExists = OwnerComp.GetBlackboardComponent()->GetValueAsBool("NeutralCampsExist");
	requestMoveID = FAIRequestID(0);
	

	if (campGoal == EReasonForGoingToCamp::RGC_Capturing)
	{
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("GoingForWin") && !OwnerComp.GetBlackboardComponent()->GetValueAsBool("FoundNearbyEnemyCamp"))
		{
			return EBTNodeResult::Failed;
		}

		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("ReachedCamp"))
			return EBTNodeResult::Succeeded;

		targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
		
	}
	else if (campGoal == EReasonForGoingToCamp::RGC_Recruiting)
	{
		
		targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("RecruitCamp"));
	}

	else if (campGoal == EReasonForGoingToCamp::RGC_DefendingCamp)
	{
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("GoingForWin"))
		{
			return EBTNodeResult::Failed;
		}
		targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("DefendCampTarget"));
	}

	else if (campGoal == EReasonForGoingToCamp::RGC_DefendingBase)
	{
		baseStructure = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("BaseStructure"));
	}
	if (hero != nullptr)
	{
		//OwnerComp.GetAIOwner()->ResumeMove(FAIRequestID::AnyRequest);
		heroStats = hero->GetHeroStats();
		sacrificeCreepAbility = hero->GetAbility(4);
		if (hero->ActorHasTag("Cyber"))
			movementAbility = hero->GetAbility(2);
		else if (hero->ActorHasTag("Diesel"))
			movementAbility = hero->GetAbility(0);
		return EBTNodeResult::InProgress;	
	}

	else
	{
		return EBTNodeResult::Failed;
	}
	

}

void UBTTask_MoveToCamp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	heroStats->UpdateStats();
	
	movementAbilityDelay += DeltaSeconds;
	if ((campGoal!= EReasonForGoingToCamp::RGC_DefendingBase && campGoal != EReasonForGoingToCamp::RGC_Recruiting)  && hero->CheckIfTowerIsBeingAttacked())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("BaseBeingAttacked", true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("BaseBeingAttacked", false);

	}

	if (heroStats->GetHealthPercent() <= 0.5f && heroStats->GetArmySize() > 0 && sacrificeCreepAbility != nullptr && sacrificeCreepAbility->CanUse() ) 
	{
		UE_LOG(LogTemp, Error, TEXT("AI Sacrificed Creep"));
		sacrificeCreepAbility->Use();
	}

	

	else if ( movementAbilityDelay >= 1.25f && movementAbility != nullptr &&  movementAbility->CanUse() && hero->SafeToJump() )
	{
		
		/*FRotator lookAtTargetRotation;
		if (campGoal != EReasonForGoingToCamp::RGC_DefendingBase)
		{
			 lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), targetCamp->GetActorLocation());
		}
		else
		{
			lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), baseStructure->GetActorLocation());
		}

		lookAtTargetRotation.Pitch = 0;
		hero->SetActorRotation(lookAtTargetRotation);
		*/
		
		//OwnerComp.GetAIOwner()->PauseMove(FAIRequestID::AnyRequest);
		movementAbility->Use();
	}


	if (heroStats->GetHealthPercent() < healthPercentageAbort && campGoal != EReasonForGoingToCamp::RGC_Recruiting)
	{
		UE_LOG(LogTemp, Error, TEXT("AI HAS LOW HP WHILE Moving to target"));
		//heroAI->ResetAITreeTaskStatus();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	}



	if (campGoal == EReasonForGoingToCamp::RGC_Capturing)
	{
		//UE_LOG(LogTemp, Error, TEXT("DISTANCE TO CAMP: %f"), hero->GetDistanceTo(targetCamp));
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("ReachedCamp"))
		{
			return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}

		else if ((hero->IsCapturing() && hero->GetCampBeingCaptured() == targetCamp) || hero->GetDistanceTo(targetCamp) <= 500.0f )
		{

			UE_LOG(LogTemp, Error, TEXT("Reached Camp"));
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", true);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		}

		else if (hero->ActorHasTag("Cyber") && targetCamp->IsDieselCapturing() && neutralCampExists && !OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsDefendingCamp")
			&& !OwnerComp.GetBlackboardComponent()->GetValueAsBool("FoundNearbyEnemyCamp"))
		{

			//heroAI->ResetAITreeTaskStatus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		}

		else if (hero->ActorHasTag("Diesel") && targetCamp->IsCyberCapturing() && neutralCampExists && !OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsDefendingCamp")
			&& !OwnerComp.GetBlackboardComponent()->GetValueAsBool("FoundNearbyEnemyCamp"))

		{
			//UE_LOG(LogTemp, Display, TEXT("ENEMY PLAYER CAPTURING TARGET CAMP"));
			//heroAI->ResetAITreeTaskStatus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);


		}

		else if (hero->CheckForNearbyInteractions())
		{
			//UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE HEADING TO CAPTURE CAMP"));
			//heroAI->ResetAITreeTaskStatus();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("FoundNearbyEnemyCamp", hero->GetNearbyEnemyCamp() != nullptr);
			
			if (hero->GetNearbyEnemyCamp() != nullptr)
			{
				if (heroAI->SafetyCheck(hero->GetNearbyEnemyCamp()))
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("NearbyEnemyCamp", hero->GetNearbyEnemyCamp());
			}
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		}


		else if (heroAI->CheckCampBeingAttacked() && !(heroAI->GetCampBeingAttacked()->AIAbondonedCamp()))
		{
			UE_LOG(LogTemp, Error, TEXT("Senses Camp Being Attacked"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}

	else if (campGoal == EReasonForGoingToCamp::RGC_Recruiting )
	{
	

		if (hero->GetDistanceTo(targetCamp) <= 700)
		{
			UE_LOG(LogTemp, Display, TEXT("AI Reached Recruitment Camp"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}



		if (hero->CheckForNearbyEnemyHero() && hero->GetDistanceTo(targetCamp) > 850.0f)
		{
			//UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE HEADING TO RECURIT CAMP"));
			//heroAI->ResetAITreeTaskStatus();
			enemyHero = hero->GetNearbyEnemyHero();
			if (((hero->GetPlayerHealthAsDecimal() > healthPercentageAbort || enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() <= 0 ||
				enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort) && enemyHero->GetArmySize() - hero->GetArmySize() <= creepDifferenceAllowed))
			{
				UE_LOG(LogTemp, Error, TEXT("Senses safe enemy nearby...was recruiting"));
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}


		else if (heroAI->CheckCampBeingAttacked() && !heroAI->GetCampBeingAttacked()->AIAbondonedCamp() && hero->GetDistanceTo(targetCamp) >1000 )
		{
			UE_LOG(LogTemp, Error, TEXT("Senses Camp Being Attacked"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}

	else if (campGoal == EReasonForGoingToCamp::RGC_DefendingCamp)

	{

		if (hero->GetDistanceTo(targetCamp) <= 700)
		{
			UE_LOG(LogTemp, Display, TEXT("AI Reached Defending Camp"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}



		if (hero->CheckForNearbyEnemyHero() )
		{
			//UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE HEADING TO RECURIT CAMP"));
			//heroAI->ResetAITreeTaskStatus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}


	}


	else if (campGoal == EReasonForGoingToCamp::RGC_GoingForWin)
	{
		if (hero->CheckForNearbyInteractions())
		{
			if (hero->GetNearbyEnemyCamp() != nullptr)
			{
				if (heroAI->SafetyCheck(hero->GetNearbyEnemyCamp()))
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("NearbyEnemyCamp", hero->GetNearbyEnemyCamp());
			}
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}

	}

	else if (campGoal == EReasonForGoingToCamp::RGC_DefendingBase)
	{
		if (hero->GetDistanceTo(baseStructure) <= 500)
		{
			UE_LOG(LogTemp, Display, TEXT("AI Reached Defending Base Structure"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}

		if (hero->CheckForNearbyEnemyHero() || hero->CheckForNearbyCreepsInArmy() || hero->CheckForNearbyOnwedCreepCamps())
		{
			//UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE HEADING TO RECURIT CAMP"));
			//heroAI->ResetAITreeTaskStatus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}


	}

}
