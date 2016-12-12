// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "DA_GameState.h"
#include "BossTower.h"


// Sets default values
ABossTower::ABossTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossTowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerStaticMesh"));
	RootComponent = BossTowerMesh;

	MoveToLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TowerMoveToLocation"));
	MoveToLocationComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	BossSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("BossSpawnLocation"));
	BossSpawnLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ETowerState = EBossTowerState::Stopped;
}

// Called when the game starts or when spawned
void ABossTower::BeginPlay()
{
	Super::BeginPlay();
	
	FStartLocation = GetActorLocation();
	FMoveToLocation = MoveToLocationComponent->GetComponentLocation();


	ADA_GameState* gameState = Cast<ADA_GameState>(GetWorld()->GameState);
	if (gameState)
	{
		gameState->SetBossTower(this);
	}
}

// Called every frame
void ABossTower::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


	if (ETowerState == EBossTowerState::Up)
	{
		//FMath::Lerp<FVector>(FStartLocation, MoveToLocation->GetComponentLocation(), DeltaTime * MovementSpeed);
		//Move Actor, compare distance with target location 
		//UE_LOG(LogTemp, Warning, TEXT("Moving Up Tower Now!!"));
		AddActorLocalOffset(FVector::UpVector * DeltaTime * MovementSpeed, false);
		if (FMoveToLocation.Z - GetActorLocation().Z <= MoveToVariance)
		{
			//spawn boss
			FActorSpawnParameters spawnParams;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			GetWorld()->SpawnActor<ACreep>(BossToSpawn, BossSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator, spawnParams);

			ETowerState = EBossTowerState::Stopped;
		}
		return;
	}
	else if(ETowerState == EBossTowerState::Down)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Moving Down Tower Now!!"));
		//FMath::Lerp<FVector>(MoveToLocation->GetComponentLocation(), FStartLocation, DeltaTime * MovementSpeed);
		//Move Actor, compare distance with target location
		AddActorLocalOffset((FVector::UpVector * DeltaTime * MovementSpeed) * -1, false);
		if (FStartLocation.Z - GetActorLocation().Z >= MoveToVariance)
		{
			ETowerState = EBossTowerState::Stopped;
		}
		return;
	}
}

void ABossTower::MoveUp()
{
	ETowerState = EBossTowerState::Up;
}

void ABossTower::MoveDown()
{
	ETowerState = EBossTowerState::Down;
}

void ABossTower::SpawnBoss()
{
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<ACreep>(BossToSpawn, BossSpawnLocation->GetComponentLocation(), FRotator::ZeroRotator, spawnParams);
}

