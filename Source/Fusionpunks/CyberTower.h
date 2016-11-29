// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TowerBase.h"
#include "CyberTower.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ACyberTower : public ATowerBase
{
	GENERATED_BODY()
	
public:

	ACyberTower();

	void SpawnProjectiles() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

private:
	
	
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* turretBase;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* turretCone;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* turretGun;
	//UPROPERTY(EditDefaultsOnly)
		//class UParticleSystemComponent* beam;
	UPROPERTY(EditDefaultsOnly, Category = ShootPositions)
		USceneComponent* shootPos1;
	UPROPERTY(EditDefaultsOnly, Category = ShootPositions)
		USceneComponent* shootPos2;
	UPROPERTY(EditDefaultsOnly, Category = ShootPositions)
		USceneComponent* shootPos3;
	UPROPERTY(EditDefaultsOnly, Category = ShootPositions)
		USceneComponent* shootPos4;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf <class AProjectile> projectileClass;

	class AProjectileTowerDamage* towerDMG;
	FVector sourceLocation;

protected:
	virtual void CleanUp() override;
	UFUNCTION()
		virtual void TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
