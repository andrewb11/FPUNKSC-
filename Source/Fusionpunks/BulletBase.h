// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

UCLASS()
class FUSIONPUNKS_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletBase();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	FORCEINLINE float GetBulletSpeed() const { return speed; }

	void Fire(float Speed, FVector Direction);

protected:
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* bulletMeshComponent; 

	UPROPERTY(EditDefaultsOnly)
		UCapsuleComponent* bulletTrigger;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float damage;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float speed;

	UPROPERTY(EditDefaultsOnly)
		UProjectileMovementComponent* projectileMovementComp;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* particleSystemComp;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
