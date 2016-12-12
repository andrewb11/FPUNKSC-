// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "GameTimerWidget.h"

#define LOCTEXT_NAMESPACE "MyNamespace"

void UGameTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GameLength = 10;
	gameTimer = 10 * 60;
}
//NOTE::BRENDON - Change to timer is getting the time data from GameState
void UGameTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsPaused)
		gameTimer -= InDeltaTime;

	
	if (gameTimer <= 0)
	{
		//start overtime
	}
}

FText UGameTimerWidget::GetGameTimeAsText() const
{
	return FText::Format(LOCTEXT("GameTimeFormat", "{0} : {1}"), FText::AsNumber((int)(gameTimer / 60)), FText::AsNumber(((int)gameTimer % 60)));
}

void UGameTimerWidget::SetPaused(bool status)
{
	bIsPaused = status;
}


#undef LOCTEXT_NAMESPACE


