// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"

#include "HeroBase.h"
#include "CyberHeroCharacter.generated.h"

UCLASS(config=Game)
class ACyberHeroCharacter: public AHeroBase
{
	GENERATED_BODY()

public:
	ACyberHeroCharacter();

	// APawn interface
	//virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime);

protected:
	virtual void LevelUp() override;

private:

	
	TArray<AActor*> affectedActors;
	bool skillSelected = false;


public:
	//void DetermineClickEvent();
	//void UseSkill(AActor* enemy);
	void AddAffectedActor(AActor* enemy);
	bool IsAffected(AActor* enemy);
	void HighlightTarget(AActor* enemy, TArray<FOverlapResult> enemies);
	void UnHighlightTarget(AActor* enemy);
	void UnHighlightAll(TArray<FOverlapResult> enemies);
	//AActor* UpdateTarget();
	

//Skill Target related
private:
	TArray<FOverlapResult> skillTargetResults;
	TArray<FOverlapResult> oldTargetResults;

protected:
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UProphetUIWidget> UIWidgetClass;

	class UProphetUIWidget* UIWidget;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* SwordMeshComp;

	UFUNCTION()
		void OnSwordComponentOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};

