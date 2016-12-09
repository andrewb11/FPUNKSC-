// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Turret.generated.h"

UENUM(BlueprintType)
enum class ETurretUseType: uint8
{
	TimeLimit		UMETA(DisplayName = "TimeLimit"),
	UseAmmo			UMETA(DisplayName = "UseAmmo"),
};

UCLASS()
class FUSIONPUNKS_API ATurret : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATurret();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	void SetOwningHero(class AHeroBase* OwningHero) { owningHero = OwningHero; }

protected:
	void TurnAtRate(float Rate);
	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	void LookUpGun(float Rate);
	void RotateBase(float Rate);

protected:
	float EnergyRemaining;
	float MaxEnergy;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float bulletSpeed = 100.0f;

	//Used for how long the player can use the turret
	UPROPERTY(EditAnywhere)
		ETurretUseType TurretUseType = ETurretUseType::TimeLimit;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* TurretGunMesh;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* BulletSpawnPoint1;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* BulletSpawnPoint2;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* TurretBaseMesh;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* TurretPillarMesh;

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* HeroInRangeRadius;

	UPROPERTY(EditDefaultsOnly, Category = Prefabs)
		TSubclassOf<class ABulletBase> BulletToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = Prefabs)
		UCameraComponent* TurretCamera;

	void ReturnToHero();

	virtual void FireBullet();

	class AHeroBase* owningHero;
	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Audio
	UPROPERTY(EditDefaultsOnly, Category = Audio)
		USoundWave* ShootSound;

};
