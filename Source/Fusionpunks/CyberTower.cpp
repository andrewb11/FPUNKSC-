// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "ProjectileTowerDamage.h"
#include "HeroBase.h"
#include "Creep.h"
#include "Projectile.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "CyberTower.h"


ACyberTower::ACyberTower() 
{
	//beam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Particle"));	
	//beam->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	turretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	turretCone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretCone"));
	turretGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretGun"));
	shootPos1 = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPos1"));
	shootPos2 = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPos2"));
	shootPos3 = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPos3"));
	shootPos4 = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPos4"));
	
	turretBase->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	turretCone->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	turretGun->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	shootPos1->AttachToComponent(turretGun, FAttachmentTransformRules::KeepRelativeTransform);
	shootPos2->AttachToComponent(turretGun, FAttachmentTransformRules::KeepRelativeTransform);
	shootPos3->AttachToComponent(turretGun, FAttachmentTransformRules::KeepRelativeTransform);
	shootPos4->AttachToComponent(turretGun, FAttachmentTransformRules::KeepRelativeTransform);
	damage = 2.0f;
	damageEverySeconds = 0.1f;

	Tags.Add(TEXT("Cyber"));
}

void ACyberTower::BeginPlay()
{
	Super::BeginPlay();
	//beam->bAutoActivate = false;
	//beam->SecondsBeforeInactive = 0;
	sourceLocation = GetActorLocation();
	sourceLocation.Z += 500;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	towerDMG = GetWorld()->SpawnActor<AProjectileTowerDamage>(towerDMGPrefab, FVector::ZeroVector, FRotator::ZeroRotator,SpawnParams);

}

void ACyberTower::TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void ACyberTower::CleanUp() 
{
	towerDMG->Destroy();
}
// Called every frame
void ACyberTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (enemyUnits.Num() > 0)
	{
		//UE_LOG(LogTemp, Log, TEXT("Cyber Tower found Player"));
		if (enemyUnits[0]->IsA(ACharacter::StaticClass()))
		{
		//	UE_LOG(LogTemp, Log, TEXT("Tower Attacking Player!"));

			/*if (!beam->bWasActive) 
			{
				beam->Activate();
				beam->SetVisibility(true);
				beam->bWasDeactivated = false;
				beam->bWasActive = true;
				//UE_LOG(LogTemp, Log, TEXT("Activating Laser Beam!"));
			}*/
			
			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(turretGun->GetComponentLocation(), enemyUnits[0]->GetActorLocation());
			turretGun->SetRelativeRotation(lookAtTargetRotation);
			//beam->SetBeamSourcePoint(0, turretGun->GetComponentLocation(), 0);
			//beam->SetBeamTargetPoint(0, enemyUnits[0]->GetActorLocation(), 0);
			
			if (!bIsDealingDMG)
			{
				towerDMG->StartTimer(damageEverySeconds, enemyUnits[0]);
				bIsDealingDMG = true;
			}			
		}
	}

	else
	{

		/*beam->SetVisibility(false);
		if (!beam->bWasDeactivated) {
			beam->Deactivate();
			beam->bWasDeactivated = true;
			beam->bWasActive = false;
			UE_LOG(LogTemp, Log, TEXT("DEActivating Laser Beam!"));
		}*/

		if (bIsDealingDMG)
		{
			towerDMG->StopTimer();
			bIsDealingDMG = false;
		}
	}
}

void ACyberTower::SpawnProjectiles()
{
	if (projectileClass != NULL)
	{
		UWorld* const world = GetWorld();
		if (world)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			FVector spawnLocation = GetActorLocation();
			spawnLocation.Z += 200;
			AProjectile* projectile1 = world->SpawnActor<AProjectile>(projectileClass, shootPos1->GetComponentLocation(), FRotator(0, 0, 0), SpawnParams);
			projectile1->SetTarget(enemyUnits[0]);
			projectile1->SetDamage(damage / 4.0f);
			AProjectile* projectile2 = world->SpawnActor<AProjectile>(projectileClass, shootPos2->GetComponentLocation(), FRotator(0, 0, 0), SpawnParams);
			projectile2->SetTarget(enemyUnits[0]);
			projectile2->SetDamage(damage / 4.0f);
			AProjectile* projectile3 = world->SpawnActor<AProjectile>(projectileClass, shootPos3->GetComponentLocation(), FRotator(0, 0, 0), SpawnParams);
			projectile3->SetTarget(enemyUnits[0]);
			projectile3->SetDamage(damage / 4.0f);
			AProjectile* projectile4 = world->SpawnActor<AProjectile>(projectileClass, shootPos4->GetComponentLocation(), FRotator(0, 0, 0), SpawnParams);
			projectile4->SetTarget(enemyUnits[0]);
			projectile4->SetDamage(damage / 4.0f);


		}

	}
}
