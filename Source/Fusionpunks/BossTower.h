// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BossTower.generated.h"

UENUM(BlueprintType)
enum class EBossTowerState : uint8
{
	Down		UMETA(DisplayName = "Down"),
	Stopped		UMETA(DisplayName = "Stopped"),
	Up			UMETA(DisplayName = "Up")
};



UCLASS()
class FUSIONPUNKS_API ABossTower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossTower();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = BossTowerFunctions)
		void MoveUp();

	UFUNCTION(BlueprintCallable, Category = BossTowerFunctions)
		void MoveDown();


protected:
	FVector FStartLocation;
	FVector FMoveToLocation;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float MoveToVariance = 1.0f;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* BossTowerMesh;

	UPROPERTY(EditDefaultsOnly)
		class USceneComponent* MoveToLocationComponent;

	UPROPERTY(EditDefaultsOnly)
		class USceneComponent* BossSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		float MovementSpeed = 10;

	UFUNCTION(BlueprintCallable, Category = BossTowerFunctions)
		void SpawnBoss();

	UPROPERTY(EditDefaultsOnly, Category = Variables)
		TSubclassOf<class ACreep> BossToSpawn;

	EBossTowerState ETowerState;
	
};
