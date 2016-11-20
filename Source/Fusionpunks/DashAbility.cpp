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
			FVector opposingForce = hero->GetCharacterMovement()->Velocity;
			hero->GetCharacterMovement()->AddForce(-opposingForce);
			hero->GetCharacterMovement()->UpdateComponentVelocity();
			//hero->GetCharacterMovement()->AddForce()
			hero->GetCharacterMovement()->GroundFriction = 8.0f;
			bHasDashed = false; 
		}
	}
}

bool ADashAbility::Ability()
{
	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		bHasDashed = true; 
		DashTimer = TargetDashTime;
		hero->GetCharacterMovement()->GroundFriction = 0.0f;
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