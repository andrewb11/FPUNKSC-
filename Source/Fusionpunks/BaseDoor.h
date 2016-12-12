// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseDoor.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ABaseDoor : public AActor
{
	GENERATED_BODY()

public:
	ABaseDoor();
	virtual void BeginPlay() override;
	bool isDestroyed = false;
	FORCEINLINE float GetHpPercent() const { return currentHealth / maxHealth; }
	FORCEINLINE void  SetEnemyHero(class AHeroBase* hero) { enemyHero = hero; }
	UPROPERTY(EditDefaultsOnly, Category = Widgets)
		TSubclassOf<class UFloatingDamageWidget> FloatingDamageWidgetClass;


private:

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* mesh;
	UPROPERTY(EditDefaultsOnly)
		class UDestructibleComponent* destructMesh;
	UPROPERTY(EditDefaultsOnly, Category = Health)
		float maxHealth =100;
	
	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* structureRadius;


	UFUNCTION()
		void AfterFracture(const FVector& in, const FVector& out);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	float currentHealth;

	class AHeroBase* enemyHero;


	



};
