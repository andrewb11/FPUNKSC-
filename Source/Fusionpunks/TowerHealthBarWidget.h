// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "TowerHealthBarWidget.generated.h"

/**
 * 
 */


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETypeOfStructure : uint8
{
	TOS_Tower UMETA(DisplayName = "Tower"),
	TOS_Door UMETA(DisplayName = "Door"),
	TOS_Reactor UMETA(DisplayName = "Reactor"),

};


UCLASS()
class FUSIONPUNKS_API UTowerHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOwningStructure(AActor* structure);
	
	
private:
	AActor* owningStructure;

	class ATowerBase *tower;
	class ABaseDoor  *door;
	class ABaseReactor* reactor;

	ETypeOfStructure structureType;

	UFUNCTION(BlueprintCallable, Category = Health)
		float GetHealthPercent() const;

};
