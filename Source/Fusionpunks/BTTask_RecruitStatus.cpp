// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "CreepCamp.h"
#include "HeroBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_RecruitStatus.h"


EBTNodeResult::Type UBTTask_RecruitStatus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACreepCamp* recruitCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("RecruitCamp"));
	AHeroAIController* heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	AHeroBase* hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (heroAI != nullptr && hero!= nullptr)
	{
		heroAI->ResetAllCampsRecruitStatus();
		heroAI->ResetAllCampsSafetyStatus();
		//OwnerComp.GetBlackboardComponent()->SetValueAsBool("ShouldRecruit", false);
		recruitCamp->SetCampRecruitedStatus(true);
		hero->AIRecruited();
		UE_LOG(LogTemp, Error, TEXT("Reset After Recruting!"));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	

		
		
}
