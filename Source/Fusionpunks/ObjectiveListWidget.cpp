// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "ObjectiveListWidget.h"

ECheckBoxState  UObjectiveListWidget::CheckCaptureObjective() 
{
	if(bCaptured)
		return ECheckBoxState::Checked;
	else
	{
		if ( owner && owner->GetCapturedCamps().Num() >= 1)
		{
			bCaptured = true;
			return ECheckBoxState::Checked;
		}
		return ECheckBoxState::Unchecked;
	}
	
}

ECheckBoxState  UObjectiveListWidget::CheckRecruitObjective() 
{
	if (bRecruited)
		return ECheckBoxState::Checked;
	else
	{
		if (owner && owner->GetArmySize() >=1)
		{
			bRecruited= true;
			return ECheckBoxState::Checked;
		}
		return ECheckBoxState::Unchecked;
	}
}
ECheckBoxState  UObjectiveListWidget::CheckTowerObjective() 
{

	if (bDestroyedTower)
		return ECheckBoxState::Checked;
	else
	{
		if (owner && owner->DestroyedEnemyTower())
		{
			bDestroyedTower = true;
			return ECheckBoxState::Checked;
		}
		return ECheckBoxState::Unchecked;
	}
}
ECheckBoxState  UObjectiveListWidget::CheckDoorObjective() 
{
	if (bDestroyedDoor)
		return ECheckBoxState::Checked;
	else
	{
		if (owner && owner->DestroyedEnemyDoor())
		{
			bDestroyedDoor = true;
			return ECheckBoxState::Checked;
		}
		return ECheckBoxState::Unchecked;
	}
}
ECheckBoxState  UObjectiveListWidget::CheckReactorObjective() 
{
	if (bDestroyedReactor)
		return ECheckBoxState::Checked;
	else
	{
		if (owner && owner->DestroyedEnemyReactor())
		{
			bDestroyedReactor = true;
			return ECheckBoxState::Checked;
		}
		return ECheckBoxState::Unchecked;
	}
}

void UObjectiveListWidget::SetOwner(class AHeroBase* hero)
{
	owner = hero;
}
