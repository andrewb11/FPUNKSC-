// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "PlayerHealthBarWidget.h"

#define LOCTEXT_NAMESPACE "MyNamespace"

float UPlayerHealthBarWidget::GetPlayerHealthAsDecimal() const
{
	AHeroBase* MyCharacter = Cast<AHeroBase>(GetOwningPlayerPawn());

	return MyCharacter->GetCurrentHealth() / MyCharacter->GetMaxHealth();
}

FText UPlayerHealthBarWidget::GetPlayerHealthText() const
{
	AHeroBase* MyCharacter = Cast<AHeroBase>(GetOwningPlayerPawn());

	if (IsValid(MyCharacter))
	{
		return FText::Format(LOCTEXT("HealthFormat", "Health: {0} / {1}"),
			FText::AsNumber(MyCharacter->GetCurrentHealth()),
			FText::AsNumber(MyCharacter->GetMaxHealth()));
	}
	return FText::AsNumber(0);

}

#undef LOCTEXT_NAMESPACE