// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "Ability_PossessCreeps.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Creep.h"
#include "BTTask_DecideHowToApproachHero.h"


EBTNodeResult::Type UBTTask_DecideHowToApproachHero::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr && heroAI != nullptr)
	{

		ability0 = hero->GetAbility(0);
		ability1 = hero->GetAbility(1);
		ability2 = hero->GetAbility(2);
		ability3 = hero->GetAbility(3);
		if (hero->ActorHasTag("Cyber"))
		{
			possessCreeps = Cast<AAbility_PossessCreeps>(hero->GetAbility(3));
		}
		heroStats = hero->GetHeroStats();
		attackTarget = Cast<AHeroBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		campTarget = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
		healingWell = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("HealingWell"));
		//OwnerComp.GetBlackboardComponent()->SetValueAsBool("IgnoreHeroMode", false);
		enemyCreep = nullptr;

		if (attackTarget->IsRespawning())
		{
			return EBTNodeResult::Failed;
		}

		if (attackTarget != nullptr && campTarget!= nullptr)
		{

			if (OwnerComp.GetAIOwner()->GetPawn()->ActorHasTag("Cyber"))
				teamCampType = ECampType::CT_Cyber;
			else
				teamCampType = ECampType::CT_Diesel;

			if (hero->GetCurrentHealth() <= healthPercentRequired)
			{
				approachStatus = EApproachStatus::AS_EscapingToBase;
			
			}

			else if ((hero->IsCapturing() || hero->GetDistanceTo(campTarget) <= 850.0f) && campTarget->GetCampType() != teamCampType || (OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsDefendingCamp") && hero->GetDistanceTo(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("DefendCampTarget"))) <= 850.0f))
			{	
				
				if(OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsDefendingCamp"))
					campTarget = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("DefendCampTarget"));

				approachStatus = EApproachStatus::AS_DefendingCamp;
				UE_LOG(LogTemp, Error, TEXT("Defensive State"));
				
			}

			else if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("AgressiveMode"))
			{
				
				approachStatus = EApproachStatus::AS_AgressiveChase;
				UE_LOG(LogTemp, Error, TEXT("Agressive State activated because AI in agressive mode"));
			}
			else if ( (attackTarget->GetArmySize() - hero->GetArmySize() <= creepDifferenceAllowed
				&& attackTarget->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() <= healthPercentDifferenceAllowed
				&& attackTarget->GetLevel() - hero->GetLevel() <= levelDifferenceAllowed) || 
				attackTarget->GetPlayerHealthAsDecimal() <= healthPercentRequired || OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsDefendingCamp"))
			{			
				approachStatus = EApproachStatus::AS_AgressiveChase;	
				UE_LOG(LogTemp, Error, TEXT("Agressive State"));
			}

			else
			{
				approachStatus = EApproachStatus::AS_AgressiveChase;
				//OwnerComp.GetBlackboardComponent()->SetValueAsBool("IgnoreHeroMode", true);
				//UE_LOG(LogTemp, Error, TEXT("Ignore Hero Mode"));
				//return EBTNodeResult::Failed;
				
			}
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
			
		}
		UE_LOG(LogTemp, Error, TEXT("Targets are null"));
		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero/Hero AI in Decide how to approach"));
		return EBTNodeResult::Failed;
	}
	hero->SetCreepAttacking(false);
	hero->SetHeroAttacking(false);
}

void UBTTask_DecideHowToApproachHero::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(attackTarget->IsRespawning())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (hero->GetPlayerHealthAsDecimal() <= healthPercentRequired && attackTarget->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() >0 &&
		attackTarget->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() > healthPercentDifferenceAllowed)
	{
		UE_LOG(LogTemp, Error, TEXT("Health to low to engage"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	

	if (approachStatus == EApproachStatus::AS_DefendingCamp)
	{
		if (attackTarget->IsCapturing() || attackTarget->GetDistanceTo(campTarget) <= 850.0f || attackTarget->GetPlayerHealthAsDecimal() <= 0.2f)
		{
			if ((hero->IsHeroAttacking() || !hero->IsCreepAttacking()))
			{
				if (hero->CheckForNearbyEnemyHero())
				{
					if (hero->GetDistanceTo(attackTarget) >= 300)
					{
						if (hero->ActorHasTag("Cyber"))
						{
							
							if (attackTarget->GetArmySize() >= 5 && possessCreeps != nullptr &&  possessCreeps->CanUse() && !possessCreeps->bIsCasting && !possessCreeps->bIsChanneling)
							{
								OwnerComp.GetAIOwner()->StopMovement();
								possessCreeps->Use();

							}
							else if (ability1 != nullptr && ability1->CanUse())
							{
								ability1->Use();
							}

							else if (ability2 != nullptr &&  ability2->CanUse())
							{
								ability2->Use();
							}
					
							if(!possessCreeps->bIsChanneling && !possessCreeps->bIsCasting)
							{
								OwnerComp.GetAIOwner()->MoveToActor(attackTarget, 50, false, true, false);
							}



						}

						else if (hero->ActorHasTag("Diesel"))
						{

							if (attackTarget->GetArmySize() >= 1 && ability2 != nullptr && ability2->CanUse())
							{
								ability2->CanUse();

							}
							else if (ability0 != nullptr && ability0->CanUse())
							{
								ability0->Use();
							}

							OwnerComp.GetAIOwner()->MoveToActor(attackTarget, 50, false, true, false);
						}

							
					}
					else if ((hero->ActorHasTag("Cyber") && (!possessCreeps->bIsChanneling && !possessCreeps->bIsCasting)) || hero->ActorHasTag("Diesel"))
					{
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					}
				}
				else
				{
					if (hero->GetDistanceTo(campTarget) >= 600)
					{

						if (hero->ActorHasTag("Cyber"))
						{

							if (ability2 != nullptr &&  ability2->CanUse())
							{
								ability2->Use();
							}
						}

						OwnerComp.GetAIOwner()->MoveToActor(campTarget, 500, true, true, false);
					}
					else
					{
						
						hero->SetHeroAttacking(false);
						FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
					}

				}
			}
			else if (hero->IsCreepAttacking())
			{
				if (hero->CheckForNearbyCreepsInArmy())
				{
					if (enemyCreep == nullptr || enemyCreep->GetBIsDead())
						enemyCreep = hero->GetClosestEnemyCreep();

					if (hero->GetDistanceTo(enemyCreep) >= 300)
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

						else if (hero->ActorHasTag("Diesel"))
						{

							if (attackTarget->GetArmySize() >= 1 && ability2 != nullptr && ability2->CanUse())
							{
								ability2->CanUse();

							}
							else if (ability0 != nullptr && ability0->CanUse())
							{
								ability0->Use();
							}

						}
						OwnerComp.GetAIOwner()->MoveToActor(enemyCreep, 50, false, true, false);
					}
				else
					{
					//UE_LOG(LogTemp, Error, TEXT("IN RANGE OF ENEMY HERO"));
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", enemyCreep);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					}
			}
			else
			{
				hero->SetCreepAttacking(false);
			}
		}
	}

		else
		{
			if (hero->GetDistanceTo(campTarget) >= 600)
			{
				OwnerComp.GetAIOwner()->MoveToActor(campTarget, 500, true, true, false);
				
				
				if (hero->ActorHasTag("Cyber"))
				{

					 if (ability2 != nullptr &&  ability2->CanUse())
					{
						ability2->Use();
					}
				}


			}
			else
			{
				/*	if (hero->CheckForNearbyEnemyHero())
				{
				FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), attackTarget->GetActorLocation());
				lookAtTargetRotation.Pitch = 0;
				hero->SetActorRotation(lookAtTargetRotation);
				} */
				//UE_LOG(LogTemp, Error, TEXT("IN RANGE OF DEFENSIVE CAMP"));
				hero->SetHeroAttacking(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}

		}

	}
	else if (approachStatus == EApproachStatus::AS_AgressiveChase)
	{
		if (hero->IsHeroAttacking() || !hero->IsCreepAttacking())
		{
			if (hero->CheckForNearbyEnemyHero())
			{
				if (hero->GetDistanceTo(attackTarget) >= 300)
				{
					if (hero->ActorHasTag("Cyber"))
					{
						if (attackTarget->GetArmySize() >= 5 && possessCreeps != nullptr &&  possessCreeps->CanUse() && !possessCreeps->bIsCasting && !possessCreeps->bIsChanneling)
						{
							OwnerComp.GetAIOwner()->StopMovement();
							possessCreeps->Use();

						}
						else if (ability1 != nullptr && ability1->CanUse())
						{
							ability1->Use();
						}

						else if (ability2 != nullptr &&  ability2->CanUse())
						{
							ability2->Use();
						}
						
						if (!possessCreeps->bIsChanneling && !possessCreeps->bIsCasting)
						{
							OwnerComp.GetAIOwner()->MoveToActor(attackTarget, 50, false, true, false);
						}

					}
					else if (hero->ActorHasTag("Diesel"))
					{

						if (attackTarget->GetArmySize() >= 1 && ability2 != nullptr && ability2->CanUse())
						{
							ability2->CanUse();

						}
						else if (ability0 != nullptr && ability0->CanUse())
						{
							ability0->Use();
						}

						OwnerComp.GetAIOwner()->MoveToActor(attackTarget, 50, false, true, false);
					}
				}
				else if  ( (hero->ActorHasTag("Cyber") && (!possessCreeps->bIsChanneling && !possessCreeps->bIsCasting)) || hero->ActorHasTag("Diesel"))
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}

			else
			{
				hero->SetHeroAttacking(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}

		}

		else if (hero->IsCreepAttacking())
		{
			if (hero->CheckForNearbyEnemyCreeps())
			{
				if (enemyCreep == nullptr || enemyCreep->GetBIsDead())
					enemyCreep = hero->GetClosestEnemyCreep();

				if (hero->GetDistanceTo(enemyCreep) >= 300)
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

					else if (hero->ActorHasTag("Diesel"))
					{

						if (attackTarget->GetArmySize() >= 1 && ability2 != nullptr && ability2->CanUse())
						{
							ability2->CanUse();

						}
						else if (ability0 != nullptr && ability0->CanUse())
						{
							ability0->Use();
						}

					}

					OwnerComp.GetAIOwner()->MoveToActor(enemyCreep, 50, false, true, false);
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("IN RANGE OF ENEMY HERO"));
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", enemyCreep);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}
			else
			{
				hero->SetCreepAttacking(false);
			}

		}
	}

	
}