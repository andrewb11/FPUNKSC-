// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "DashAbility.h"


void ADashAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DashTimer -= DeltaSeconds;

	//BRENDON NOTE::Move to time handler event!
	if (DashTimer <= 0 && bHasDashed)
	{
		AHeroBase* hero = Cast<AHeroBase>(GetOwner());
		if (hero)
		{
			hero->bIsDashing = false;
			FVector opposingForce = hero->GetCharacterMovement()->Velocity;
			hero->GetCharacterMovement()->AddForce(-opposingForce);
			hero->GetCharacterMovement()->UpdateComponentVelocity();
			//hero->GetCharacterMovement()->AddForce()
			hero->GetCharacterMovement()->GroundFriction = 8.0f;
			bHasDashed = false; 

			UBoolProperty* boolProp = FindField<UBoolProperty>(hero->GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsDash"));
			if (boolProp)
			{
				boolProp->SetPropertyValue_InContainer(hero->GetMesh()->GetAnimInstance(), false);
			}
		}
	}
}

bool ADashAbility::Ability()
{
	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		UBoolProperty* boolProp = FindField<UBoolProperty>(hero->GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsDash"));
		if (boolProp)
		{
			boolProp->SetPropertyValue_InContainer(hero->GetMesh()->GetAnimInstance(), true);
		}

		bHasDashed = true; 
		DashTimer = TargetDashTime;
		//set to can do damage
		hero->bIsDashing = true;

		hero->GetCharacterMovement()->GroundFriction = 0.0f;
		//false originally, changed bool to true to see if ability works with AI
		hero->GetCharacterMovement()->AddImpulse(hero->GetActorForwardVector() * DashForce, false);

		//hero->GetCharacterMovement()->AddForce(hero->GetActorUpVector() * -DashForce);
		hero->GetCharacterMovement()->UpdateComponentVelocity();
		
		//UBoolProperty* boolProp = FindField<UBoolProperty>(hero->GetMesh()->GetAnimInstance()->GetClass(), TEXT("MeleeAttacking"));
		//if (boolProp)
		//{
		//	bIsAttacking = true;
		//	boolProp->SetPropertyValue_InContainer(GetMesh()->GetAnimInstance(), true);
		//	//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
		//	GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AHeroBase::StopAttacking, 1.0f, false);
		//}

		return true;
	}
	return false; 
}