// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "ProjectileTowerDamage.h"
#include "DieselTower.h"


// Sets default values
ADieselTower::ADieselTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	turretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	turretCone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretCone"));
	turretGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretGun"));
	turretBase->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	turretCone->AttachToComponent(turretBase, FAttachmentTransformRules::KeepRelativeTransform);
	turretGun->AttachToComponent(turretCone, FAttachmentTransformRules::KeepRelativeTransform);
	shootPos1 = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPos1"));
	shootPos2 = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPos2"));
	shootPos1->AttachToComponent(turretGun, FAttachmentTransformRules::KeepRelativeTransform);
	shootPos2->AttachToComponent(turretGun, FAttachmentTransformRules::KeepRelativeTransform);

	damage = 20.0f;
	damageEverySeconds = 1.0f;

	Tags.Add(TEXT("Diesel"));
}

// Called when the game starts or when spawned
void ADieselTower::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	towerDMG = GetWorld()->SpawnActor<AProjectileTowerDamage>(towerDMGPrefab, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
}

void ADieselTower::TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::TriggerExit(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (enemyUnits.Num() == 0)
	{
		if (IsValid(towerDMG))
		{
			towerDMG->StopTimer();
		}
		
		bIsDealingDMG = false;
	}
}
void ADieselTower::CleanUp() 
{
	towerDMG->Destroy();
}

// Called every frame
void ADieselTower::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if (enemyUnits.Num() > 0)
	{
		if (enemyUnits[0]->IsA(ACharacter::StaticClass()))
		{
			//UE_LOG(LogTemp, Log, TEXT("Attacking Player!"));
			if (!bIsDealingDMG)
			{
				
				towerDMG->StartTimer(damageEverySeconds, enemyUnits[0]);
				bIsDealingDMG = true;
			}

			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(turretGun->GetComponentLocation(), enemyUnits[0]->GetActorLocation());
			turretGun->SetRelativeRotation(FRotator(lookAtTargetRotation.Pitch, turretGun->RelativeRotation.Yaw, turretGun->RelativeRotation.Roll));
			//FRotator lookAtTargetRotation2 = UKismetMathLibrary::FindLookAtRotation(turretCone->GetComponentLocation(), enemyUnits[0]->GetActorLocation());
			turretCone->SetRelativeRotation(FRotator(turretCone->RelativeRotation.Pitch, lookAtTargetRotation.Yaw - turretBase->RelativeRotation.Yaw, turretCone->RelativeRotation.Roll));
			
		}
	}
	else 
	{
		if (bIsDealingDMG)
		{
			towerDMG->StopTimer();
			bIsDealingDMG = false;
		}
	}

}

void ADieselTower::SpawnProjectiles()
{
	if (whatToSpawn != NULL)
	{
		UWorld* const world = GetWorld();
		if (world)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			AProjectile* projectile1 = world->SpawnActor<AProjectile>(whatToSpawn, shootPos1->GetComponentLocation(), FRotator(0, 0, 0), SpawnParams);
			projectile1->SetTarget(enemyUnits[0]);
			projectile1->SetDamage(damage / 2.0f);
			AProjectile* projectile2 = world->SpawnActor<AProjectile>(whatToSpawn, shootPos2->GetComponentLocation(), FRotator(0, 0, 0), SpawnParams);
			projectile2->SetTarget(enemyUnits[0]);
			projectile2->SetDamage(damage / 2.0f);
		}
	}
}

