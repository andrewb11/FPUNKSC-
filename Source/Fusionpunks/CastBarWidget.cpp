// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Ability_PossessCreeps.h"
#include "CastBarWidget.h"


float UCastBarWidget::GetCastTimeLeftAsDecimal() const
{
	AAbility_PossessCreeps* owner = Cast<AAbility_PossessCreeps>(OwningActor);
	if (owner)
	{
		if (owner->bIsCasting)
		{
			return owner->GetCastTimeLeftAsDecimal();
		}
		else if (owner->bIsChanneling)
		{
			return owner->GetChannelTimeLeftAsDecimal();
		}
	}
	return 1;
}

FText UCastBarWidget::GetAbilityName() const
{
	AAbility_PossessCreeps* owner = Cast<AAbility_PossessCreeps>(OwningActor);
	return(owner ? FText::FromName(owner->GetAbilityName()) : FText::FromName(" "));
}
