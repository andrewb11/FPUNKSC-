// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "ProjectileTowerDamage.h"
#include <Classes/Kismet/KismetMathLibrary.h>
#include "TowerBase.generated.h"

UCLASS(Abstract)
class FUSIONPUNKS_API ATowerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATowerBase();

	virtual void SpawnProjectiles(AActor* enemy) { check(0 && "You must override this"); }

	virtual void BeginPlay() override;
	

	FORCEINLINE void StopTimer() { towerDMG->StopTimer(); }
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;
	
	void RemoveFromTargetList(AActor* enemy);

	float GetHpPercent();

	FORCEINLINE void SetIsDealingDamage(bool status) { bIsDealingDMG = status; }

	UPROPERTY(EditDefaultsOnly, Category = Damage)
		float damage;


	bool CheckForEnemyHero();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int XPKillReward;

	class AProjectileTowerDamage* towerDMG;

	virtual void CleanUp() { check(0 && "You must override this") };

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<class UFloatingDamageWidget> FloatingDamageWidgetClass;
	
	FVector myLocation;

	float spawnTimer;
	bool bHasSpawed = false;

	//UPROPERTY(EditDefaultsOnly)
	//	UHealthBarWidgetComponent *healthBar;

	UPROPERTY(EditDefaultsOnly)
		UShapeComponent* radius;

	UPROPERTY(EditDefaultsOnly)
		UShapeComponent* rootCldr;

	UFUNCTION()
		virtual void TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		virtual void TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	TArray<AActor*> enemyUnits;

	

	UPROPERTY(EditDefaultsOnly)
		float maxHP = 100;

	float currHP;




	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AEffectOverTimeBase> towerDMGPrefab;

	
	
	UPROPERTY(EditDefaultsOnly, Category = Damage)
		float damageEverySeconds;

	UPROPERTY(EditDefaultsOnly, Category = HeroClass)
		TSubclassOf<class AHeroBase> teamheroClass;
	UPROPERTY(EditDefaultsOnly, Category = HeroClass)
		TSubclassOf<class AHeroBase> enemyheroClass;
	class AHeroBase* teamHero;
	class AHeroBase* enemyHero;


	bool bIsDealingDMG = false;

	



	//APlayerController* playerCam;

	//AActor* playerChar;

};
