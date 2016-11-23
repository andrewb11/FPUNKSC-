// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "ChainLightning.h"
#include "ChainLightningSpawner.h"

AActor* AChainLightningSpawner::UpdateTarget()
{
	//UE_LOG(LogTemp, Display, TEXT("Skill Used"));
	/*if (oldTargetResults.Num() > 0)
	{
		UnHighlightAll(oldTargetResults);
	} */

	AActor *closestEnemy;
	FCollisionObjectQueryParams obejctQP;
	obejctQP.AddObjectTypesToQuery(Creeps);
	obejctQP.AddObjectTypesToQuery(Hero);
	//obejctQP.AddObjectTypesToQuery()
	//Overlap multi by channel as a sphere (for pick ups?)

	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(GetOwner());
	QueryParameters.OwnerTag = TEXT("Player");

	TArray<FOverlapResult> skillTargetResults;
	GetWorld()->OverlapMultiByObjectType(skillTargetResults,
		GetOwner()->GetActorLocation(),
		FQuat(),
		obejctQP,
		FCollisionShape::MakeSphere(750),
		QueryParameters);
	//oldTargetResults = skillTargetResults;

	if (skillTargetResults.Num() == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("No Units Nearby"));
		return NULL;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Unit Found"));

		TArray<AActor*> enemies;
		for (int i = 0; i < skillTargetResults.Num(); i++)
		{
			if (!skillTargetResults[i].GetActor()->ActorHasTag("Cyber"))
			{
				enemies.Add(skillTargetResults[i].GetActor());
			}
		}

		if (enemies.Num() > 0)
		{
			closestEnemy = enemies[0];

			for (int i = 0; i < enemies.Num(); i++)
			{
				if (GetDistanceTo(enemies[i]) <= GetDistanceTo(closestEnemy))
				{
					closestEnemy = enemies[i];
				}
			}

			//HighlightTarget(closestEnemy, skillTargetResults);
			return closestEnemy;
		}
		UE_LOG(LogTemp, Display, TEXT("No Enemies Nearby"));
		return NULL;
	}
}

bool AChainLightningSpawner::Ability()
{
	AActor* enemy = UpdateTarget();
	if (enemy != nullptr)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = GetOwner();
		spawnParams.Instigator = Instigator;

		FVector spawnLoc;
		spawnLoc = GetOwner()->GetActorLocation();
		spawnLoc.Z = spawnLoc.Z + 200;
		AChainLightning* lightning = GetWorld()->SpawnActor
			<AChainLightning>(chainLightningAbility,
				spawnLoc,
				FRotator::ZeroRotator, spawnParams);
		lightning->AddAffectedActor(enemy);
		lightning->SetBeamPoints(GetOwner(), enemy);
		lightning->Use();
		return true;
	}

	return false;
}


