// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseReactor.generated.h"

UCLASS()
class FUSIONPUNKS_API ABaseReactor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseReactor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	bool isDestroyed = false;
	FORCEINLINE float GetHpPercent() const { return currentHealth / maxHealth; }
	FORCEINLINE void  SetEnemyHero(class AHeroBase* hero) { enemyHero = hero; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = Health)
		float maxHealth = 100;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	float currentHealth;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
		TSubclassOf<class UFloatingDamageWidget> FloatingDamageWidgetClass;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* structureRadius;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		UStaticMeshComponent* ReactorMesh;

	class AHeroBase* enemyHero;
	
	
};
