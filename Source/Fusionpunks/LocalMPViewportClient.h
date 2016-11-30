// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameViewportClient.h"
#include "LocalMPViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ULocalMPViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed = 1.f, bool bGamepad = false) override;
	virtual bool InputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples = 1, bool bGamepad = false) override;
	
};
