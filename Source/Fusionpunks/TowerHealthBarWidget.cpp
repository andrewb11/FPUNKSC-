// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "TowerBase.h"
#include "BaseDoor.h"
#include "BaseReactor.h"
#include "TowerHealthBarWidget.h"


void UTowerHealthBarWidget::SetOwningStructure(AActor* structure)
{

	if (structure->IsA(ATowerBase::StaticClass())) 
	{
		tower = Cast<ATowerBase>(structure);
		structureType = ETypeOfStructure::TOS_Tower;
	}
	else if (structure->IsA(ABaseDoor::StaticClass()))
	{
		door = Cast<ABaseDoor>(structure);
		structureType = ETypeOfStructure::TOS_Door;
	}
	else if (structure->IsA(ABaseReactor::StaticClass()))
	{
		reactor = Cast<ABaseReactor>(structure);
		structureType = ETypeOfStructure::TOS_Reactor;
	}
}

float UTowerHealthBarWidget::GetHealthPercent() const
{
	
	if (structureType == ETypeOfStructure::TOS_Tower && tower)
	{
		return tower->GetHpPercent();
	}

	else if(structureType ==  ETypeOfStructure::TOS_Door && door)
	{
		return door->GetHpPercent();
	}
	

	else if (structureType == ETypeOfStructure::TOS_Reactor && reactor)
	{
		return reactor->GetHpPercent();
	}


	return 0;
}