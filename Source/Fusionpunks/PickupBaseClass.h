// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickupBaseClass.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Health					UMETA(DisplayName = "Health"),
	TowerBarExperience		UMETA(DisplayName = "TowerBarXP"),
	SpawnCreepArmy			UMETA(DisplayName = "SpawnCreeps")
};

UCLASS()
class FUSIONPUNKS_API APickupBaseClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBaseClass();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* TriggerBox;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		EPickupType PickupType = EPickupType::Health;
	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


protected:
	UPROPERTY(EditDefaultsOnly, Category = Variables)
		int creepsToSpawn = 5;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float towerExperience;





};
