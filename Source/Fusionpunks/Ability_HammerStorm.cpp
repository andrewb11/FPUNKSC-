// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "Ability_HammerStorm.h"


void AAbility_HammerStorm::BeginPlay()
{
	Super::BeginPlay();

	rotator = FRotator::FRotator(0, 1, 0);
	rotator *= rotationSpeed;
	owningActor = GetOwner();
}

void AAbility_HammerStorm::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsSpinning)
	{
		AHeroBase* hero = Cast<AHeroBase>(owningActor);
		if (hero && !hero->bIsRespawning)
		{
			owningActor->AddActorLocalRotation(rotator * DeltaSeconds);
		}
	}
}


bool AAbility_HammerStorm::Ability()
{
	bIsSpinning = true;

	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		hero->bIsAttacking = true; 
		hero->TurnWithInput(false);
		hero->Slow(1 - movementSpeedPercentage);

		//change animation state machine 
		UBoolProperty* boolProp = FindField<UBoolProperty>(hero->GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsSpinning"));
		if (boolProp)
		{
			boolProp->SetPropertyValue_InContainer(hero->GetMesh()->GetAnimInstance(), true);
			//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
		}
	}
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AAbility_HammerStorm::RestoreMovement, abilityDuration, true);
	//GetWorld()->GetTimerManager().SetTimer(spinTimerHandle, this, &AAbility_HammerStorm::SpinActor, timeUntilNextRotation, true); 
	return true;
}

void AAbility_HammerStorm::RestoreMovement()
{
	bIsSpinning = false;

	if (timerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	}
	/*if (spinTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(spinTimerHandle);
	}*/

	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		hero->bIsAttacking = false;
		hero->TurnWithInput(true);
		hero->RestoreWalkSpeed();

		//change animation state machine 
		UBoolProperty* boolProp = FindField<UBoolProperty>(hero->GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsSpinning"));
		if (boolProp)
		{
			boolProp->SetPropertyValue_InContainer(hero->GetMesh()->GetAnimInstance(), false);
			//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
		}
	}
}

void AAbility_HammerStorm::SpinActor()
{
	if (owningActor)
	{
		owningActor->AddActorLocalRotation(rotator);
	}
}