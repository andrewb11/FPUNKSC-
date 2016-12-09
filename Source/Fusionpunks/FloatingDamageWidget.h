// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "FloatingDamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UFloatingDamageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetIncDamage(float Damage) { incDamage = Damage; }

	UFUNCTION(BlueprintCallable, Category = WidgtFunctions)
		void SetOwningActor(AActor* Owner) { OwningActor = Owner; }

	UFUNCTION(BlueprintCallable, Category = WidgtFunctions)
		AActor* GetOwningActor() const { return OwningActor; }

	UPROPERTY(BlueprintReadWrite, Category = FloatingDamageWidgetVariables)
	AActor* Instigator;

private:
	UFUNCTION(BlueprintCallable, Category = Combat)
		FText GetIncDamage() const;
	
    float incDamage;
	AActor* OwningActor;
	
	
};
