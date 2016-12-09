// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "Ability_Leap.h"



bool AAbility_Leap::Ability()
{
	if (GetOwner()->IsA(ACharacter::StaticClass()))
	{
		AHeroBase* hero = Cast<AHeroBase>(GetOwner());
		if (hero)
		{
			//Play Take Damage Aniamtion
			UBoolProperty* boolProp = FindField<UBoolProperty>(hero->GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsLeap"));
			if (boolProp)
			{
				boolProp->SetPropertyValue_InContainer(hero->GetMesh()->GetAnimInstance(), true);
			}

			orignalGravityScale = hero->GetCharacterMovement()->GravityScale;
			//false, false = add to current velocity || true, true = override current velocity 
			hero->GetCharacterMovement()->GravityScale = gravityScale;
		/*	hero->GetCharacterMovement()->Velocity = FVector::ZeroVector;
			hero->GetCharacterMovement()->UpdateComponentVelocity();*/

			FVector forwardVector = hero->GetActorForwardVector();
			forwardVector *= forwardForce;
			forwardVector.Z += upwardsForce;
			hero->LaunchCharacter(forwardVector, true, true);

			GetWorld()->GetTimerManager().SetTimer(leapTimerHandle, this, &AAbility_Leap::RevertGravityScale, leapTime, false);
			return true;
		}	
	}

	return false; 
}

void AAbility_Leap::RevertGravityScale()
{
	if (leapTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(leapTimerHandle);

	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		hero->GetCharacterMovement()->GravityScale = orignalGravityScale;
	}
}
