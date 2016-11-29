// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CastBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UCastBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOwner(AActor* Owner) { OwningActor = Owner; }

	UFUNCTION(BlueprintCallable, Category = CastBarWidgetFunctions)
		float GetCastTimeLeftAsDecimal() const;
	UFUNCTION(BlueprintCallable, Category = CastBarWidgetFunctions)
		FText GetAbilityName() const;

protected:
	AActor* OwningActor;
	
	
};
