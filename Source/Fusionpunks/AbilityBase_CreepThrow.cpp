// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "Creep.h"
#include "CreepAIController.h"
#include "AbilityBase_CreepThrow.h"

AAbilityBase_CreepThrow::AAbilityBase_CreepThrow()
{
	particleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectedCreepLaunch"));
	pathLifeTime = 5.0f;
	timeInterval = 0.05f;
}

void AAbilityBase_CreepThrow::BeginPlay()
{
	Super::BeginPlay();

	creepThrowState = ECreepThrowState::CTS_FindCreepToThrow;
	pointCount = pathLifeTime / timeInterval;
	hero = Cast<AHeroBase>(GetOwner());
}


void AAbilityBase_CreepThrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//show where the creep will end up
	if(creepThrowState == ECreepThrowState::CTS_CreepInHand)
	{
		DrawTrajectory();
	}
}

void AAbilityBase_CreepThrow::DrawTrajectory()
{
	FVector a, b;

	FVector forwardVector = hero->GetActorForwardVector();
	forwardVector *= forwardLaunchForce;
	forwardVector.Z += upwardsLaunchForce;

	/*FCollisionQueryParams queryParams;
	queryParams.bTraceComplex = true; 
	queryParams.AddIgnoredActor(this);

	FCollisionResponseParams responseParams;
	responseParams.DefaultResponseParam;*/

	for (int i = 0; i < pointCount; i++)
	{
		a = GetSegmentAtTime(hero->GetCreepAttachment()->GetComponentLocation(), forwardVector, FVector::FVector(0, 0, -980), i * timeInterval);
		b = GetSegmentAtTime(hero->GetCreepAttachment()->GetComponentLocation(), forwardVector, FVector::FVector(0, 0, -980), (i + 1) * timeInterval);
		//GetWorld()->LineTraceSingleByChannel(trajectoryHitResult, a, b, ECollisionChannel::ECC_Visibility, queryParams, responseParams);
		DrawDebugLine(GetWorld(), a, b, FColor::Red, false, -1.0f, (uint8)'\000', 5.0f);
	}

	
}

//call multiple times for diff time segments
FVector AAbilityBase_CreepThrow::GetSegmentAtTime(FVector StartLocation, FVector InitialVelocity, FVector Gravity, float Time)
{
	return StartLocation + InitialVelocity * Time + (Time * Time) * Gravity;
}

bool AAbilityBase_CreepThrow::Ability()
{
	if (creepThrowState == ECreepThrowState::CTS_FindCreepToThrow)
	{
		AHeroBase* hero = Cast<AHeroBase>(GetOwner());
		if (hero)
		{
			//DO we have a creep army?
			if (hero->GetArmySize() > 0)
			{
				//Find closest creep in range 
				for (int i = 0; i < hero->GetArmySize(); i++)
				{
					FVector vector = hero->GetCreepArmyArray()[i]->GetActorLocation() - hero->GetActorLocation();
					float distance = vector.Size();

					//is the creep within a certain range? --> if so grab it
					if (distance < rangeCheck)
					{
						creepToThrow = hero->GetCreepArmyArray()[i];
						break; //break out of for loop
					}
				}
				//NOTE::BRENDON TO DO: Switch Creep a State where he is playing a helpless animation, maybe add in a sound when you pick him up and throw him
				if (creepToThrow != nullptr)
				{
					ACreepAIController* creepController = Cast<ACreepAIController>(creepToThrow->GetController());
					if (creepController)
					{
						hero->GetCharacterMovement()->MaxWalkSpeed = 0;
						creepController->StopBehaviorTree();
						UE_LOG(LogTemp, Warning, TEXT("Attaching Creep to hero!"));
						//pick up the creep
						creepToThrow->SetActorEnableCollision(false);
						creepToThrow->AttachToComponent(hero->GetCreepAttachment(), FAttachmentTransformRules::KeepRelativeTransform);
						//creepToThrow->SetActorRelativeLocation(FVector::ZeroVector);
						creepToThrow->SetActorLocation(hero->GetCreepAttachment()->GetComponentLocation());
						creepToThrow->SetActorRotation(FRotator::ZeroRotator);
						creepThrowState = ECreepThrowState::CTS_CreepInHand;
						return false;
					}
					UE_LOG(LogTemp, Warning, TEXT("Failed to find creep AI Controller, From AbilityBase_CreepThrow"));
					return false; //failed to get / stop AI Controller 
				} 
				UE_LOG(LogTemp, Warning, TEXT("creepToThrow was nullptr, From AbilityBase_CreepThrow"));
				return false; 
			}
			UE_LOG(LogTemp, Warning, TEXT("No Creeps in army to throw!"));
			return false; //No Creeps In Army to throw
		}
		return false;
	}
	else //must have found a creep to throw
	{
		AHeroBase* hero = Cast<AHeroBase>(GetOwner());
		if (hero)
		{
			UE_LOG(LogTemp, Warning, TEXT("Throwing Creep!!"));
			//if we press again then launch the creep
			//Unattach the creep from the hero and launch it
			FVector forwardVector = hero->GetActorForwardVector();
			forwardVector *= forwardLaunchForce;
			forwardVector.Z += upwardsLaunchForce;

			ACreepAIController* creepController = Cast<ACreepAIController>(creepToThrow->GetController());
			if (creepController)
			{
				creepController->UnPossess();
				creepController->Destroy();
			}
			
			creepToThrow->HideHealthBar();
			creepToThrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			creepToThrow->GetMesh()->SetSimulatePhysics(true);
			creepToThrow->GetMesh()->SetPhysicsLinearVelocity(forwardVector);
			//creepToThrow->LaunchCharacter(forwardVector, false, false);
			creepToThrow->SetActorEnableCollision(true);
			
			//make "Gib" function??
			creepToThrow->TakeDamage(100000000, FDamageEvent::FDamageEvent(), hero->GetController(), hero);
			if (creepToThrow)
			{
				hero->RemoveCreepFromArmy(creepToThrow);
			}
			creepToThrow = nullptr;

			hero->RestoreWalkSpeed();

			creepThrowState = ECreepThrowState::CTS_FindCreepToThrow;
			return true;
		}
		return false; //error finding hero
	}

}

