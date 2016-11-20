// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "SlowOverTime.h"
#include "Ability_SlowEnemyCharacter.h"



bool AAbility_SlowEnemyCharacter::Ability()
{
	//Search for hero in range
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams obejctQP;
	obejctQP.AddObjectTypesToQuery(Hero);

	TArray<FOverlapResult> Results;

	GetWorld()->OverlapMultiByObjectType(Results, GetOwner()->GetActorLocation(), FQuat(), obejctQP, FCollisionShape::MakeSphere(range), QueryParams);

	for (int i = 0; i < Results.Num(); i++)
	{
		AHeroBase* hero = Cast<AHeroBase>(Results[i].GetActor());
		if (hero && !hero->Tags.Contains("Cyber"))
		{
			//check if owner is facing the target


			//if facing the target spawn a slow over time effect on the enemy 
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = GetOwner();
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ASlowOverTime* slowOverTime = GetWorld()->SpawnActor<ASlowOverTime>(slowOverTimeClass, hero->GetActorLocation(), FRotator::ZeroRotator, spawnParams);
			slowOverTime->SetSlowPercentage(SlowPercentage);
			slowOverTime->StartTimer(SlowDuration, hero);
			return true;
		}
	}
	return false; 
}
