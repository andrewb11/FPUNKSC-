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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime);

protected:
	virtual void LevelUp() override;

private:
	UPROPERTY(EditAnywhere, category = Ability)
		TSubclassOf<class AAbilityBase> chainLightningAbility;
	
	TArray<AActor*> affectedActors;
	bool skillSelected = false;


public:
	void AddAffectedActor(AActor* enemy);
	bool IsAffected(AActor* enemy);
	void HighlightTarget(AActor* enemy, TArray<FOverlapResult> enemies);
	void UnHighlightTarget(AActor* enemy);
	void UnHighlightAll(TArray<FOverlapResult> enemies);	

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

	virtual void OnDeath() override; 
public:
	virtual void OnRespawn() override;
	
	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
	virtual void InitializeHUD() override;
	
	virtual void MeleeAttack() override;
//Melee Attack Stuff
protected:

	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		float launchForceOne =  700;
	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		float launchForceTwo = 700;
	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		float launchForceThree = 4500;

	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		UAnimMontage* MeleeAttackOne;
	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		UAnimMontage* MeleeAttackTwo;
	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		UAnimMontage* MeleeAttackThree;

public:
	virtual void ChainCombo() override; 

protected:
	virtual void PossessTurret() override;
};

