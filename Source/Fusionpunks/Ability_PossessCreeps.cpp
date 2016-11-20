// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "CastBarWidget.h"
#include "Ability_PossessCreeps.h"

AAbility_PossessCreeps::AAbility_PossessCreeps()
{
	bIsCasting = false;
	bIsChanneling = false;

	castTimer = 0;
	AbilityName = FName::FName("Possess Creeps");
}
void AAbility_PossessCreeps::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<AHeroBase>(GetOwner());
	channelTimer = ChannelDuration;
}

void AAbility_PossessCreeps::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsCasting)
	{
		//if character moves while casting
		if (owner->GetCharacterMovement()->Velocity.Size() > 0)
		{
			bIsCasting = false; 
			castTimer = 0;
			//hide widget
			if (castBarWidget)
			{
				castBarWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			return;
		}
		//if we don't move..
		castTimer += DeltaSeconds;
		if (castTimer >= CastTime)
		{
			castTimer = 0;
			bIsCasting = false;
			//Check Area Around the Cast & convert enemy creeps to your team
			FCollisionQueryParams QueryParameters;
			QueryParameters.AddIgnoredActor(this);
			QueryParameters.OwnerTag = TEXT("Player");

			FCollisionObjectQueryParams obejctQP;
			obejctQP.AddObjectTypesToQuery(Hero);

			TArray<FOverlapResult> actorsFound;
			GetWorld()->OverlapMultiByObjectType(actorsFound,GetActorLocation(),FQuat(),obejctQP,FCollisionShape::MakeSphere(AbilityRadius), QueryParameters);

			for(int i = 0; i < actorsFound.Num(); i++)
			{
				//the first hero we find should be the only hero we want to 
				AHeroBase* hero = Cast<AHeroBase>(actorsFound[i].GetActor());
				if (hero && !hero->ActorHasTag("Cyber"))
				{
					EnemyHero = hero;
					UE_LOG(LogTemp, Warning, TEXT("Enemy Hero Found! (Possess Creep Ability)"))
					EnemyHero->CreepArmyChangeTeam(true);
					bIsChanneling = true;
					return;
				}
			}
			bIsChanneling = false; 
			if (castBarWidget)
			{
				castBarWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		return;
	}
	else if (bIsChanneling)
	{
		//if character moves while casting
		if (owner->GetCharacterMovement()->Velocity.Size() > 0)
		{
			bIsChanneling = false;
			channelTimer = ChannelDuration;
			if (castBarWidget)
			{
				castBarWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			
			//revert enemy creeps back to original team
			if (EnemyHero)
			{
				EnemyHero->CreepArmyChangeTeam(false);
				EnemyHero = nullptr;
			}

		}
		channelTimer -= DeltaSeconds;
		if (channelTimer <= 0)
		{
			bIsChanneling = false;
			channelTimer = ChannelDuration;
			if (castBarWidget)
			{
				castBarWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			//revert enemy creeps back to original team
			if (EnemyHero)
			{
				EnemyHero->CreepArmyChangeTeam(false);
				EnemyHero = nullptr;
			}
		}
	}
}

bool AAbility_PossessCreeps::Ability()
{
	//if character is moving cannot cast ability
	if (owner->GetCharacterMovement()->Velocity.Size() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Cast While Moving!"));
		return false; 
	}

	bIsCasting = true; 
	//Spawn Widget if it's not already in memory
	if(CastTimerWidgetClass != nullptr && castBarWidget == nullptr)
	{
		castBarWidget = CreateWidget<UCastBarWidget>(GetWorld(), CastTimerWidgetClass);
		castBarWidget->SetOwner(this);
		castBarWidget->AddToViewport();
		return true;
	}
	//otherwise just show it
	if (castBarWidget)
	{
		castBarWidget->SetVisibility(ESlateVisibility::Visible);
	}
	return true; 
}

