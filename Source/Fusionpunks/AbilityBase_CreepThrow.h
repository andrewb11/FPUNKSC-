// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "AbilityBase_CreepThrow.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECreepThrowState : uint8
{
	CTS_FindCreepToThrow UMETA(DisplayName = "NoCreepInHand"),
	CTS_CreepInHand      UMETA(DisplayName = "CreepInHand"),
	CTS_PreparingToThrow    UMETA(DisplayName = "PreparingToThrow")
};

UCLASS()
class FUSIONPUNKS_API AAbilityBase_CreepThrow : public AAbilityBase
{
	GENERATED_BODY()
	
public:
	AAbilityBase_CreepThrow();
	virtual void BeginPlay() override;
	virtual bool Ability() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float rangeCheck;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float InitialUpwardsLaunchForce = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float InitialForwardLaunchForce = 75.0f;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float MaxUpwardsLaunchForce = 1600.0f;
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float MaxForwardLaunchForce = 2000.0f;

	float upwardsLaunchForce;
	float forwardLaunchForce;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float ForwardLaunchIncreasePerFrame = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float UpwardLaunchIncreasePerFrame = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float characterSlowPercent;
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float timeUntilDetonate = 3.0f;


	UPROPERTY(EditDefaultsOnly, Category = BeamParticleSystem)
		UParticleSystemComponent* particleSystemComp;
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float pathLifeTime;
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float timeInterval;

	float pointCount;

	void DrawTrajectory();
	FVector GetSegmentAtTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float Time);
	FHitResult trajectoryHitResult;
	class AHeroBase* hero;

	//https://www.youtube.com/watch?v=Fkp50urxShg&index=2&list=PL8DfVXEyUb9XCje9eyKtLvVqL8Tt9VHF2 (8:40)

	ECreepThrowState creepThrowState;
	class ACreep* creepToThrow;
	
	
};
