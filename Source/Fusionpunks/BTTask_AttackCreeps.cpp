// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "AIController.h"
#include "HeroBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creep.h"
#include "Kismet/KismetMathLibrary.h"
#include "Ability_HammerStorm.h"
#include "BTTask_AttackCreeps.h"

EBTNodeResult::Type UBTTask_AttackCreeps::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	isAttacking = false;
	if (hero != nullptr)
	{
		heroStats = hero->GetHeroStats();
		target = Cast<ACreep>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		ability0 = hero->GetAbility(0);
		ability1 = hero->GetAbility(1);
		ability2 = hero->GetAbility(2);
		ability3 = hero->GetAbility(3);

		if (hero->ActorHasTag("Diesel"))
		{
			hammerStormClass = Cast<AAbility_HammerStorm>(hero->GetAbility(1));
		}

		if (target)
		{
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
		}
		UE_LOG(LogTemp, Error, TEXT("Cant Find Creep To Attack"));

		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero In Attack Creep Task"));
		return EBTNodeResult::Failed;
	}
}

void UBTTask_AttackCreeps::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (target != nullptr && !hammerStormClass->bIsSpinning)
	{
		FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), target->GetActorLocation());
		lookAtTargetRotation.Pitch = 0;
		hero->SetActorRotation(lookAtTargetRotation);
	}


	if (!isAttacking)
	{
		GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackCreeps::AttackOnTimer, attackSpeed, true, 0);
		isAttacking = true;
		UE_LOG(LogTemp, Error, TEXT("Starting Attack Timer!"));
	}
		
				
	if (target->GetHealthAsDecimal()<=0 || hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort || hero->GetDistanceTo(target) > 300 )
	{
		UE_LOG(LogTemp, Error, TEXT("STOP ATTACK TIMER"));
		target = nullptr;
		if (attackTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}

void UBTTask_AttackCreeps::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	if (attackTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);

}
void UBTTask_AttackCreeps::AttackOnTimer() 
{
	if (hero->ActorHasTag("Cyber"))
	{

		if (ability0 != nullptr && ability0->CanUse())
		{
			ability0->Use();
		}

		else if (ability1 != nullptr && ability1->CanUse())
		{
			ability1->Use();
		}

		else if (target != nullptr)
			hero->MeleeAttack();
			//hero->Attack(target);
	}

	else if (hero->ActorHasTag("Diesel"))
	{

		if (ability1 != nullptr && ability1->CanUse())
		{
			ability1->Use();
		}

		else if (target != nullptr)
			hero->MeleeAttack();
			//hero->Attack(target);
	}

	
}