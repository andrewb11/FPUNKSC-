// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "FloatingDamageWidget.h"


FText UFloatingDamageWidget::GetIncDamage() const
{
	return FText::AsNumber(incDamage);
}

