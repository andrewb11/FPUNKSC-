// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "TowerBase.h"
#include "Creep.h"
#include "BaseDoor.h"
#include "BaseReactor.h"
#include "HeroAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_AttackEnemyHero.h"
#include "Ability_HammerStorm.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTTask_AttackEnemyHero::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	enemyHero = nullptr;
	enemyCreep = nullptr;
	enemyTower = nullptr;
	if (OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")->IsA(AHeroBase::StaticClass()))
	{
		enemyHero = Cast<AHeroBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		targetType = ETargetType::TT_Hero;
	}
	else if (OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")->IsA(ACreep::StaticClass()))
	{
		enemyCreep = Cast<ACreep>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		targetType = ETargetType::TT_Creep;
	}

	else if (OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")->IsA(ATowerBase::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Attack Target is a tower!"));
		enemyTower = Cast<ATowerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		targetType = ETargetType::TT_Tower;
	}
	else if (OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")->IsA(ABaseDoor::StaticClass()))
	{
		enemyBaseDoor= Cast<ABaseDoor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		targetType = ETargetType::TT_BaseDoor;
	}
	else if (OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")->IsA(ABaseReactor::StaticClass()))
	{
		enemyBaseReactor = Cast<ABaseReactor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		targetType = ETargetType::TT_BaseReactor;
	}


	//OwnerComp.GetAIOwner()->StopMovement();
	isAttacking = false;
	if (hero != nullptr)
	{
		if (hero->ActorHasTag("Diesel"))
		{
			hammerStormClass = Cast<AAbility_HammerStorm>(hero->GetAbility(1));
		}
		ability0 = hero->GetAbility(0);
		ability1 = hero->GetAbility(1);
		ability2 = hero->GetAbility(2);
		ability3 = hero->GetAbility(3);

		bNotifyTick = true;
		return EBTNodeResult::InProgress;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero or Target"));
		return EBTNodeResult::Failed;
	}
}
void UBTTask_AttackEnemyHero::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (targetType == ETargetType::TT_Hero && enemyHero != nullptr)
	{
		if (!isAttacking)
		{
			GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackEnemyHero::AttackHeroOnTimer, attackSpeed, true, 0);
			isAttacking = true;
			UE_LOG(LogTemp, Error, TEXT("Starting Attack Hero Timer!"));
		}

		if (isAttacking && ((hero->ActorHasTag("Diesel") && !hammerStormClass->bIsSpinning) || hero->ActorHasTag("Cyber")))
		{
			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), enemyHero->GetActorLocation());
			lookAtTargetRotation.Pitch = 0;
			hero->SetActorRotation(lookAtTargetRotation);
		}

		if ((hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort && enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() > 0 &&
			enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() > healthPercentageAbort) || hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning ||
			hero->GetDistanceTo(enemyHero) > 300 || enemyHero->bIsRespawning || enemyHero->GetPlayerHealthAsDecimal() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("STOP ATTACK Hero TIMER"));
			enemyHero = nullptr;

			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
				hero->SetHeroAttacking(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
	else if (targetType == ETargetType::TT_Creep && enemyCreep != nullptr)
	{

		if (!isAttacking && enemyCreep != nullptr)
		{
			GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackEnemyHero::AttackCreepOnTimer, attackSpeed, true, 0);
			isAttacking = true;
			UE_LOG(LogTemp, Error, TEXT("Starting Attack Creep Timer!"));
		}

		if (isAttacking && enemyCreep != nullptr && ((hero->ActorHasTag("Diesel") && !hammerStormClass->bIsSpinning) || hero->ActorHasTag("Cyber")) )
		{
			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), enemyCreep->GetActorLocation());
			lookAtTargetRotation.Pitch = 0;
			hero->SetActorRotation(lookAtTargetRotation);
		}

		if (hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort || hero->GetDistanceTo(enemyCreep) > 300 || enemyCreep->GetHealthAsDecimal() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("STOP ATTACK Creep TIMER"));
			enemyCreep = nullptr;

			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}

	else if (targetType == ETargetType::TT_Tower && enemyTower != nullptr)
	{
	
		if (!isAttacking && enemyTower != nullptr)
		{
			GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackEnemyHero::AttackTowerOnTimer, attackSpeed, true, 0);
			isAttacking = true;
			UE_LOG(LogTemp, Error, TEXT("Starting Attack Tower Timer!"));
		}

		if (isAttacking && enemyTower != nullptr && ((hero->ActorHasTag("Diesel") && !hammerStormClass->bIsSpinning) || hero->ActorHasTag("Cyber")))
		{
			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), enemyTower->GetActorLocation());
			lookAtTargetRotation.Pitch = 0;
			hero->SetActorRotation(lookAtTargetRotation);
		}

		if ((enemyTower->GetHpPercent() > 0.2f && hero->CheckForNearbyEnemyHero()) ||  hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort || hero->GetDistanceTo(enemyTower) > 500 || enemyTower->GetHpPercent() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("STOP ATTACK TOWER TIMER"));
			enemyTower = nullptr;

			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
				//hero->SetCreepAttacking(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}

	else if (targetType == ETargetType::TT_BaseDoor && enemyBaseDoor != nullptr)
	{
		if (!isAttacking && enemyBaseDoor!= nullptr)
		{
			GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackEnemyHero::AttackBaseOnTimer, attackSpeed, true, 0);
			isAttacking = true;
			UE_LOG(LogTemp, Error, TEXT("Starting Attack Base door Timer!"));
		}

		if (isAttacking && enemyBaseDoor != nullptr && ((hero->ActorHasTag("Diesel") && !hammerStormClass->bIsSpinning) || hero->ActorHasTag("Cyber")))
		{
			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), enemyBaseDoor->GetActorLocation());
			lookAtTargetRotation.Pitch = 0;
			hero->SetActorRotation(lookAtTargetRotation);
		}

		if ((enemyBaseDoor->GetHpPercent() > 0.2f && hero->CheckForNearbyEnemyHero()) || hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort || hero->GetDistanceTo(enemyBaseDoor) > 500 || enemyBaseDoor->isDestroyed)
		{
			UE_LOG(LogTemp, Error, TEXT("STOP ATTACK base door TIMER"));
			enemyBaseDoor = nullptr;

			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
				//hero->SetCreepAttacking(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}


	else if (targetType == ETargetType::TT_BaseReactor && enemyBaseReactor != nullptr)
	{
		if (!isAttacking && enemyBaseReactor != nullptr)
		{
			GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackEnemyHero::AttackBaseOnTimer, attackSpeed, true, 0);
			isAttacking = true;
			UE_LOG(LogTemp, Error, TEXT("Starting Attack Base Reactor Timer!"));
		}

		if (isAttacking && enemyBaseReactor != nullptr && ((hero->ActorHasTag("Diesel") && !hammerStormClass->bIsSpinning) || hero->ActorHasTag("Cyber")))
		{
			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), enemyBaseReactor->GetActorLocation());
			lookAtTargetRotation.Pitch = 0;
			hero->SetActorRotation(lookAtTargetRotation);
		}

		if ((enemyBaseReactor->GetHpPercent() > 0.2f && hero->CheckForNearbyEnemyHero()) || hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort || hero->GetDistanceTo(enemyBaseReactor) > 500 || enemyBaseReactor->isDestroyed)
		{
			UE_LOG(LogTemp, Error, TEXT("STOP ATTACK base Reactor TIMER"));
			enemyBaseReactor = nullptr;

			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
				//hero->SetCreepAttacking(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}






	else if(targetType == ETargetType::TT_None)
	{
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

}
void UBTTask_AttackEnemyHero::AttackHeroOnTimer()
{
	if (hero == nullptr || hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning ||
		hero->GetDistanceTo(enemyHero) > 300 || enemyHero->bIsRespawning || enemyHero->GetPlayerHealthAsDecimal() <= 0)
	{
		if (attackTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
		}
	}

	else if (hero->ActorHasTag("Cyber"))
	{
		if (ability0 != nullptr && ability0->CanUse())
		{
			ability0->Use();
		}

		else if (ability1 != nullptr &&  ability1->CanUse())
		{
			ability1->Use();
		}

		else
		{
			hero->MeleeAttack();
			//hero->Attack(enemyHero);
		}
	}

	else if (hero->ActorHasTag("Diesel"))
	{

		if (ability3 != nullptr && ability3->CanUse())
		{
			ability3->Use();
		}
		else if (ability1 != nullptr && ability1->CanUse())
		{
			ability1->Use();
		}	
		else
		{
			hero->MeleeAttack();
			//hero->Attack(enemyHero);
		}
	}


	
}
void UBTTask_AttackEnemyHero::AttackCreepOnTimer()
{
	if (enemyCreep == nullptr|| hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning || hero->GetDistanceTo(enemyCreep) > 300 || enemyCreep->GetHealthAsDecimal() <= 0)
	{
		if (attackTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
		}
	}
	
	else if (hero->ActorHasTag("Cyber"))
	{
		if (ability0 != nullptr && ability0->CanUse())
		{
			ability0->Use();
		}

		else if (ability1 != nullptr &&  ability1->CanUse())
		{
			ability1->Use();
		}

		else
		{
			hero->MeleeAttack();
			//hero->Attack(enemyCreep);
		}
	}

	else if (hero->ActorHasTag("Diesel"))
	{
		if (ability1 != nullptr && ability1->CanUse())
		{
			ability1->Use();
		}
		else
		{
			hero->MeleeAttack();
			//hero->Attack(enemyCreep);
		}
	}
	
}
void UBTTask_AttackEnemyHero::AttackTowerOnTimer()
{
	if (enemyTower == nullptr || hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning || hero->GetDistanceTo(enemyTower) > 500 || enemyTower->GetHpPercent() <= 0)
	{
		if (attackTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
		}
	}

	else
	{
		hero->MeleeAttack();
		//hero->Attack(enemyTower);
	}
	

}

void UBTTask_AttackEnemyHero::AttackBaseOnTimer()
{
	if (targetType == ETargetType::TT_BaseDoor)
	{
		
		if (enemyBaseDoor == nullptr || hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning || hero->GetDistanceTo(enemyBaseDoor) > 500 || enemyBaseDoor->isDestroyed)
		{
			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
			}
		}

		else
			hero->MeleeAttack();


	}

	else if (targetType == ETargetType::TT_BaseReactor)
	{
		if (enemyBaseReactor == nullptr || hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning || hero->GetDistanceTo(enemyBaseReactor) > 500 || enemyBaseReactor->isDestroyed)
		{
			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
			}
		}

		else
			hero->MeleeAttack();
	}


}
