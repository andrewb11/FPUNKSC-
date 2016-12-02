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
private:

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* mesh;
	UPROPERTY(EditDefaultsOnly)
		class UDestructibleComponent* destructMesh;
	UPROPERTY(EditDefaultsOnly, Category = Health)
		float maxHealth =100;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	float currentHealth;
};