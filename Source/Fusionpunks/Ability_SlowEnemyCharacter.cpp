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
		AHeroBase* enemyHero = Cast<AHeroBase>(Results[i].GetActor());
		if (enemyHero && enemyHero->ActorHasTag("Cyber"))
		{
			//Update Anim State Machine
			AHeroBase* hero = Cast<AHeroBase>(GetOwner());
			if (hero)
			{
				//Play Take Damage Animation
				
				UBoolProperty* boolProp = FindField<UBoolProperty>(hero->GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsSlow"));
				if (boolProp)
				{
					boolProp->SetPropertyValue_InContainer(hero->GetMesh()->GetAnimInstance(), true);
				}
			}

			FActorSpawnParameters spawnParams;
			spawnParams.Owner = hero;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ASlowOverTime* slowOverTime = GetWorld()->SpawnActor<ASlowOverTime>(slowOverTimeClass, hero->GetActorLocation(), FRotator::ZeroRotator, spawnParams);
			slowOverTime->SetSlowPercentage(SlowPercentage);
			slowOverTime->StartTimer(SlowDuration, enemyHero);
			return true;
		}
	}
	return false; 
}
