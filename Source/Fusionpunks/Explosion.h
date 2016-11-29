// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class FUSIONPUNKS_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		bool bFriendlyFire = false;
	
protected:
	FName team;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float Damage = 25.0f;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly)
		URadialForceComponent* ExplosionForceComponent;

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* ExplosionTrigger;

	UPROPERTY(EditDefaultsOnly, Category = Audio)
		USoundWave* ExplosionSound;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* ThisComp, 
			class AActor* OtherActor, 
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult &SweepResult);
};
