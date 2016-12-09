// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "CreepCamp.h"
#include "DustParticleEffect.h"
#include "HeroBase.h"
#include "DieselHeroCharacter.generated.h"

UCLASS(config=Game)
class ADieselHeroCharacter : public AHeroBase
{

public:
	GENERATED_BODY()

	ADieselHeroCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void LevelUp() override;
	
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* WeaponMesh;

protected:
	UPROPERTY(EditAnywhere, Category = "AbilityEffects")
		TSubclassOf<AActor> dustParticleSystemClass;

//UI Stuff
protected:
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class URankUIWidget> UIWidgetClass;

	class URankUIWidget* UIWidget;

	//for Weapon Overlap (Hammer)
	UFUNCTION()
	void OnHammerComponentOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//Temp until animation is fixed
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* HammerCollisionTrigger;
public:
	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		virtual void InitializeHUD() override;

	virtual void MeleeAttack() override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		float launchForceOne = 700;
	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		float launchForceTwo = 700;

	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		UAnimMontage* MeleeAttackOne;
	UPROPERTY(EditDefaultsOnly, Category = MeleeAtatck)
		UAnimMontage* MeleeAttackTwo;
public:
	virtual void ChainCombo() override;

protected:
	virtual void OnDeath() override;
public:
	virtual void OnRespawn() override;


protected:
	virtual void PossessTurret() override;
};

