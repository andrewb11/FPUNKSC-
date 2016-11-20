// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "NeutralCreep.h"
#include "CyberCreep.h"
#include "DieselCreep.h"
#include "CreepCamp.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
ACreepCamp::ACreepCamp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerCollider"));
	sphereTrigger->SetSphereRadius(280.0f, true);
	sphereTrigger->bGenerateOverlapEvents = true;
	RootComponent = sphereTrigger;
	//sphereTrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Create and Set the Static Mesh Component
	campMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CampMesh"));
	//const ConstructorHelpers::FObjectFinder<UStaticMesh>campStaticMesh(TEXT("StaticMesh'/Game/CreepCamp/Models/mk6_goerge.mk6_goerge'"));
	//campMesh->SetStaticMesh(campStaticMesh.Object);
	campMesh->bGenerateOverlapEvents = false;
	campMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	campMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Create our ring around the camp
	//ringMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CaptureRing"));
	//const ConstructorHelpers::FObjectFinder<UStaticMesh> ringStaticMesh(TEXT("StaticMesh'/Game/CreepCamp/Models/CreepCampCircle.CreepCampCircle'"));
	//ringMesh->SetStaticMesh(ringStaticMesh.Object);
	//ringMesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 3.25f));
	//ringMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 5.5f));
	//ringMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//create & Set box trigger for capturing the camp
	
	

	lampPostMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampPostMesh"));
	lampPostMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//starting status
	campType = ECampType::CT_Neutral;

	captureVariables = FCaptureVariables::FCaptureVariables();
	spawningVariables = FSpawningVariables::FSpawningVariables();

	//speed at which camp rotates
	ringRotationSpeed = 25.0f;
	ringMaterialAlpha = 0.5f;
	ringMaterialAlphaSpeed = 1.0f;
	bCountUp = true;
	
	FlagMaterials.SetNum(NUMFLAGS, false);

}

// Called when the game starts or when spawned
void ACreepCamp::BeginPlay()
{
	Super::BeginPlay();
	/*if(ringMesh!= nullptr)
		ringRotation = ringMesh->GetComponentRotation();*/
	sphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACreepCamp::OnOverlapBegin);
	sphereTrigger->OnComponentEndOverlap.AddDynamic(this, &ACreepCamp::OnOverlapEnd);

	lampPostMesh->SetMaterial(0, lampPostMaterial);
	
	//set locations for initial spawning 
	creep1SpawnLocation = FVector(this->GetActorLocation().X + 500, this->GetActorLocation().Y, this->GetActorLocation().Z + 125);
	creep2SpawnLocation = FVector(this->GetActorLocation().X - 500, this->GetActorLocation().Y, this->GetActorLocation().Z + 125);
	creep3SpawnLocation = FVector(this->GetActorLocation().X, this->GetActorLocation().Y + 500, this->GetActorLocation().Z + 125);
	creepSpawnArray.Add(creep1SpawnLocation);
	creepSpawnArray.Add(creep2SpawnLocation);
	creepSpawnArray.Add(creep3SpawnLocation);

	if (campType == ECampType::CT_Cyber)
	{
		team = FName::FName(TEXT("Cyber"));

		lampPostMesh->SetMaterial(1, FlagMaterials[0]);

		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		for (int i = 0; i < spawningVariables.startCreepAmount; i++)
		{
			ACyberCreep* cyberCreep = (ACyberCreep*)GetWorld()->SpawnActor<ACyberCreep>
				(cyberCreepRef,
					creepSpawnArray[i % creepSpawnArray.Num()],
					FRotator::ZeroRotator,
					spawnParameters);
		
			if (cyberCreep->IsValidLowLevel())
			{
				cyberCreep->SetCreepCampHome(this, true);
				creepArray.Add(cyberCreep);
				spawningVariables.creepCount++;
			}
		}
		captureVariables.cyberCaptureProgress = captureVariables.captureTime;
		//cyberCaptureProgress = captureTime;
	}
	else if (campType == ECampType::CT_Diesel)
	{
		team = FName::FName(TEXT("Diesel"));

		lampPostMesh->SetMaterial(1, FlagMaterials[2]);

		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		for (int i = 0; i < spawningVariables.startCreepAmount; i++)
		{
			ADieselCreep* dieselCreep = (ADieselCreep*)GetWorld()->SpawnActor<ADieselCreep>
				(dieselCreepRef,
					creepSpawnArray[i % creepSpawnArray.Num()],
					FRotator::ZeroRotator,
					spawnParameters);

			if (dieselCreep->IsValidLowLevel())
			{
				dieselCreep->SetCreepCampHome(this, true);
				creepArray.Add(dieselCreep);
				spawningVariables.creepCount++;
			}
		}
		captureVariables.dieselCaptureProgress = captureVariables.captureTime;
		//dieselCaptureProgress = captureTime;
	}
	else
	{
		team = FName::FName(TEXT("Neutral"));

		lampPostMesh->SetMaterial(1, FlagMaterials[0]);

		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		for (int i = 0; i < spawningVariables.startCreepAmount; i++)
		{
			ANeutralCreep* neutralCreep = (ANeutralCreep*)GetWorld()->SpawnActor<ANeutralCreep>
				(neutralCreepRef,
					creepSpawnArray[i % creepSpawnArray.Num()],
					FRotator::ZeroRotator,
					spawnParameters);

			if (neutralCreep->IsValidLowLevel())
			{
				neutralCreep->SetCreepCampHome(this, true);
				creepArray.Add(neutralCreep);
				spawningVariables.creepCount++;
			}
		}
	}
}

// Called every frame
void ACreepCamp::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	/*if (campType == ECampType::CT_Cyber)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creep Count: %i"), spawningVariables.creepCount);
		UE_LOG(LogTemp, Warning, TEXT("Creep Count: %i"), spawningVariables.cyberCreepLimit);
	}*/
		
	//Debug Messages to print to screen
	//Could probably make an entire struct to handle this 

	if (GetWorld()->GetName() == "MechanicTesting")
	{
		GEngine->ClearOnScreenDebugMessages();
		static const FString MCreepCount(TEXT("\n\nCreep Count: "));
		static const FString MSpawnTimer(TEXT("\n\nSpawn Timer: "));
		static const FString MCaptureTimer(TEXT("\n\nCampCaptureTimer: "));

		print(MCreepCount + FString::FromInt(spawningVariables.creepCount)
			+ MSpawnTimer + FString::FromInt((int)spawningVariables.creepSpawnTimer)
			+ MCaptureTimer + FString::FromInt((int)captureVariables.captureTime));
	}
	//Rotate the ring every frame
	//ringRotation.Yaw += DeltaTime * ringRotationSpeed;
	/*if (ringMesh != nullptr)
	ringMesh->SetRelativeRotation(ringRotation);*/

	//if cyber is capturing
	if (spawningVariables.creepCount == 0 && !captureVariables.bDieselIsCapturing && captureVariables.bCyberIsCapturing
		|| !captureVariables.bDieselIsCapturing && captureVariables.bCyberIsCapturing && campType == ECampType::CT_Cyber)
	{
		////Note: Make this better
		//if (ringMaterialAlpha >= 1)
		//{
		//	bCountUp = false;
		//}
		//else if (ringMaterialAlpha <= 0.1f)
		//{
		//	bCountUp = true;
		//}

		//if (bCountUp)
		//{
		//	ringMaterialAlpha += DeltaTime * ringMaterialAlphaSpeed;
		//}
		//else if (!bCountUp	)
		//{
		//	ringMaterialAlpha -= DeltaTime * ringMaterialAlphaSpeed;
		//}
		////set ring material to fade in and out
		//if (ringMesh != nullptr)
		//	ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), ringMaterialAlpha);
		//check capture progress
		if (captureVariables.dieselCaptureProgress >= 0)
		{
			captureVariables.dieselCaptureProgress -= DeltaTime;
		}
		
		else if (captureVariables.dieselCaptureProgress <= 0 && captureVariables.dieselCaptureProgress < captureVariables.targetCaptureTime)
		{
			captureVariables.cyberCaptureProgress += DeltaTime;
		}

		if (campType != ECampType::CT_Cyber && captureVariables.cyberCaptureProgress >= captureVariables.captureTime)
		{
			SetToCyberCamp();
			DestroyAllCreeps();
		}
	}
	//is Diesel capturing?
	else if (spawningVariables.creepCount == 0 && !captureVariables.bCyberIsCapturing && captureVariables.bDieselIsCapturing 
		|| captureVariables.bDieselIsCapturing && !captureVariables.bCyberIsCapturing && campType == ECampType::CT_Diesel)
	{
		//if (ringMaterialAlpha >= 1)
		//{
		//	bCountUp = false;
		//}
		//else if (ringMaterialAlpha <= 0.1f)
		//{
		//	bCountUp = true;
		//}

		//if (bCountUp)
		//{
		//	ringMaterialAlpha += DeltaTime * ringMaterialAlphaSpeed;
		//}
		//else if (!bCountUp)
		//{
		//	ringMaterialAlpha -= DeltaTime * ringMaterialAlphaSpeed;
		//}

		////set ring material to fade in and out
		//if (ringMesh != nullptr)
		//	ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), FMath::Sin(ringMaterialAlpha));

		if (captureVariables.cyberCaptureProgress >= 0)
		{
			captureVariables.cyberCaptureProgress -= DeltaTime;
		}

		else if (captureVariables.cyberCaptureProgress <= 0 && captureVariables.cyberCaptureProgress < captureVariables.targetCaptureTime)
		{
			captureVariables.dieselCaptureProgress += DeltaTime;
		}

		if (campType != ECampType::CT_Diesel && captureVariables.dieselCaptureProgress >= captureVariables.captureTime)
		{
			SetToDieselCamp();
			DestroyAllCreeps();
		}
	}

	//Note - Brendon: Do we want to keep spawning creeps at neutral camps or have a cap???
	//spawning creeps
	//if neutral camp
	if (!captureVariables.bDieselIsCapturing && !captureVariables.bCyberIsCapturing 
		|| captureVariables.bCyberIsCapturing && campType == ECampType::CT_Cyber && !captureVariables.bDieselIsCapturing
		|| captureVariables.bDieselIsCapturing && campType == ECampType::CT_Diesel && !captureVariables.bCyberIsCapturing)
	{
		if (campType == ECampType::CT_Neutral && spawningVariables.creepCount < spawningVariables.neutralCreepLimit)
		{
			spawningVariables.creepSpawnTimer -= DeltaTime;
			if (spawningVariables.creepSpawnTimer <= 0)
			{
				FActorSpawnParameters spawnParameters;
				spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				int random = FMath::RandRange(0, 2);
				ANeutralCreep* neutralCreep = (ANeutralCreep*)GetWorld()->SpawnActor<ANeutralCreep>
					(neutralCreepRef,
						creepSpawnArray[random],
						FRotator::ZeroRotator,
						spawnParameters);

				if (neutralCreep->IsValidLowLevel())
				{
					neutralCreep->SetCreepCampHome(this, true);
					creepArray.Add(neutralCreep);
					spawningVariables.creepCount++;
					neutralCreep->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					neutralCreep->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				}

				spawningVariables.creepSpawnTimer = spawningVariables.creepSpawnTimerTarget;
			}
		}
		//if cyber camp
		else if (campType == ECampType::CT_Cyber && spawningVariables.creepCount < spawningVariables.cyberCreepLimit)
		{
			spawningVariables.creepSpawnTimer -= DeltaTime;

			if (spawningVariables.creepSpawnTimer <= 0)
			{
				if (campType == ECampType::CT_Cyber)
				{
					FActorSpawnParameters spawnParameters;
					spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					int random = FMath::RandRange(0, 2);

					ACyberCreep* cyberCreep = (ACyberCreep*)GetWorld()->SpawnActor<ACyberCreep>
						(cyberCreepRef,
							creepSpawnArray[random],
							FRotator::ZeroRotator,
							spawnParameters);

					if (cyberCreep->IsValidLowLevel())
					{
						cyberCreep->SetCreepCampHome(this, true);
						creepArray.Add(cyberCreep);
						spawningVariables.creepCount++;
						cyberCreep->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						cyberCreep->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					}
				}
				spawningVariables.creepSpawnTimer = spawningVariables.creepSpawnTimerTarget;
			}
		}
		//if diesel camp
		else if (campType == ECampType::CT_Diesel && spawningVariables.creepCount < spawningVariables.dieselCreepLimit)
		{
			spawningVariables.creepSpawnTimer -= DeltaTime;

			if (spawningVariables.creepSpawnTimer <= 0)
			{
				FActorSpawnParameters spawnParameters;
				spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				int random = FMath::RandRange(0, 2);
				ADieselCreep* dieselCreep = (ADieselCreep*)GetWorld()->SpawnActor<ADieselCreep>
					(dieselCreepRef,
						creepSpawnArray[random],
						FRotator::ZeroRotator,
						spawnParameters);

				if (dieselCreep->IsValidLowLevel())
				{
					dieselCreep->SetCreepCampHome(this, true);
					creepArray.Add(dieselCreep);
					spawningVariables.creepCount++;
					dieselCreep->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					dieselCreep->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				}

				spawningVariables.creepSpawnTimer = spawningVariables.creepSpawnTimerTarget;
			}
		}
	}

	//update the capture target depending on how many creeps are in the camp
	//if (campType != ECampType::CT_Neutral && spawningVariables.creepCount > 3)
	//{	
	//	/*                              @3 Creeps:              */
	//	captureVariables.captureTime = captureVariables.targetCaptureTime + (spawningVariables.creepCount * captureVariables.captureTimeMultiplier);

	//	//update capture progress bar for whoever owns the camp
	//	if (!captureVariables.bCyberIsCapturing && !captureVariables.bDieselIsCapturing)
	//	{
	//		if (campType == ECampType::CT_Cyber)
	//		{
	//			captureVariables.cyberCaptureProgress = captureVariables.captureTime;
	//		}
	//		else
	//		{
	//			captureVariables.dieselCaptureProgress = captureVariables.captureTime;
	//		}
	//	}
	//}

	//NOTE - BRENDON: Can change multiplier and target timer in Blueprints, have to balance this 
	/*														 @ 3 Creeps
	(5 seconds		    +						   (3   *   1.5)) = 9 seconds
	@ 4 Creeps
	(5 seconds			+						(4		*	 1.5)) = 11 seconds */
	//spawningVariables.creepSpawnTimer = spawningVariables.creepSpawnTimerTarget + (spawningVariables.creepCount * spawningVariables.creepSpawnTimerMultiplier);

}//end of tick

//On Trigger Function 
void ACreepCamp::OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Something Entered Camp Trigger"));

	if (OtherActor->Tags.Contains("CyberPlayer") || OtherActor->Tags.Contains("DieselPlayer"))
	{
		if (Cast<AHeroBase>(OtherActor))
		{
			AHeroBase* heroChar = Cast<AHeroBase>(OtherActor);
			
			if (!OtherActor->Tags.Contains("AI"))
			{
				heroChar->ShowCampProgress(this);
				captureVariables.bPlayerIsCapturing = true;
			}
				

			heroChar->SetIsCapturing(true, this);
		}	

		if (OtherActor->Tags.Contains("CyberPlayer"))
		{
			captureVariables.bCyberIsCapturing = true;
		}

		if (OtherActor->Tags.Contains("DieselPlayer"))
		{
			captureVariables.bDieselIsCapturing = true;

		}

		
		
	}
}


void ACreepCamp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Something Exited Camp Trigger"));

	if (OtherActor->Tags.Contains("CyberPlayer") || OtherActor->Tags.Contains("DieselPlayer"))
	{
		if (Cast<AHeroBase>(OtherActor))
		{
			AHeroBase* heroChar = Cast<AHeroBase>(OtherActor);
			if (!OtherActor->Tags.Contains("AI"))
			{
				heroChar->HideCampProgress();
				captureVariables.bPlayerIsCapturing = false;
			}
				
			heroChar->SetIsCapturing(false, this);
		}

		if (OtherActor->Tags.Contains("CyberPlayer"))
		{
			captureVariables.bCyberIsCapturing = false;
		}

		if (OtherActor->Tags.Contains("DieselPlayer"))
		{
			captureVariables.bDieselIsCapturing = false;
		}

		/*if (ringMesh != nullptr)
			ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), 0.5f);*/
	}
}

void ACreepCamp::SetToDieselCamp()
{
	team = FName::FName(TEXT("Diesel"));

	lampPostMesh->SetMaterial(1, FlagMaterials[2]);

	//captureVariables.bDieselIsCapturing = false;
	
	//set color and transparency of ring
	/*if (ringMesh != nullptr) {
		ringMesh->SetVectorParameterValueOnMaterials(TEXT("RingColor"), FVector::FVector(0, 0, 0));
		ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), 0.5f);
	}*/

	//Play announcer notification
	if (Announcer_CampCaptured && Announcer_CampLost)
	{
		if (captureVariables.bPlayerIsCapturing)
		{
			UGameplayStatics::PlaySound2D(this, Announcer_CampCaptured);
		}
		else if (campType != ECampType::CT_Neutral)
		{
			UGameplayStatics::PlaySound2D(this, Announcer_CampLost);
		}
	}

	if (GetWorld()->GetName() != "MechanicsTesting")
	{
		dieselHero->AddToCapturedCamps(this);
		cyberHero->RemoveFromCapturedCamps(this);

		//for now
		cyberHero->UpdateHeroStats();
		dieselHero->UpdateHeroStats();
	}
	
	campType = ECampType::CT_Diesel;
}

//change camp functionality to cyber function
void ACreepCamp::SetToCyberCamp()
{
	team = FName::FName(TEXT("Cyber"));
	//captureVariables.bCyberIsCapturing = false;

	lampPostMesh->SetMaterial(1, FlagMaterials[1]);
	//set color and transparency of ring 
	/*if (ringMesh != nullptr)
	{
		ringMesh->SetVectorParameterValueOnMaterials(TEXT("RingColor"), FVector::FVector(0.0, 0.0f, 1.0f));
		ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), 0.5f);
	}*/

	//Play announcer notification
	if (Announcer_CampCaptured && Announcer_CampLost)
	{
		if (captureVariables.bPlayerIsCapturing)
		{
			UGameplayStatics::PlaySound2D(this, Announcer_CampCaptured);
		}
		else if(campType != ECampType::CT_Neutral)
		{
			UGameplayStatics::PlaySound2D(this, Announcer_CampLost);
		}
	}

	if (GetWorld()->GetName() != "MechanicsTesting")
	{
		cyberHero->AddToCapturedCamps(this);
		dieselHero->RemoveFromCapturedCamps(this);

		//for now
		cyberHero->UpdateHeroStats();
		dieselHero->UpdateHeroStats();
	}

	campType = ECampType::CT_Cyber;
}

void ACreepCamp::SetToNeutralCamp()
{
	team = FName::FName(TEXT("Neutral"));
	campType = ECampType::CT_Neutral;

	lampPostMesh->SetMaterial(1, FlagMaterials[0]);

	//set color and transparency of ring
	/*if (ringMesh != nullptr)
	{
		ringMesh->SetVectorParameterValueOnMaterials(TEXT("RingColor"), FVector::FVector(1.0f, 1.0f, 1.0f));
		ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), 0.5f);
	}*/
	//Play announcer notification

	if (GetWorld()->GetName() != "MechanicsTesting")
	{
		dieselHero->RemoveFromCapturedCamps(this);
		cyberHero->RemoveFromCapturedCamps(this);

		//for now
		cyberHero->UpdateHeroStats();
		dieselHero->UpdateHeroStats();
	}
}

void ACreepCamp::RemoveCreep(ACreep* CreepInCamp)
{
	if (creepArray.Contains<ACreep*>(CreepInCamp))
	{
		creepArray.Remove(CreepInCamp);
		spawningVariables.creepCount--;
	}
}

void ACreepCamp::DestroyAllCreeps()
{
	/*for (int i = 0; i < creepArray.Num(); i++)
	{
		if (!creepArray[i]->IsActorBeingDestroyed())
		{
			creepArray[i]->Destroy();
			creepArray[i]
		}
	}*/
	for (int i = 0; i < creepArray.Num(); i++)
	{
		if (creepArray[i] != nullptr ||creepArray[i]->IsActorBeingDestroyed() == false)
		{
			creepArray[i]->Destroy();
		}
	}
	creepArray.Empty();
	spawningVariables.creepCount = 0;
}

void ACreepCamp::LinkToHeroes(AHeroBase* hero)
{
	if (hero->GetTeam().Compare(FName::FName(TEXT("Cyber"))) == 0)
	{
		cyberHero = hero;
		UE_LOG(LogTemp, Log, TEXT("Found Cyber Hero."));
	}
	else
	{
		dieselHero = hero;
		UE_LOG(LogTemp, Log, TEXT("Found Diesel Hero."));
	}
}

ACreep* ACreepCamp::GetNextCreep(AHeroBase* Hero)
{
	const bool bSameTeam = team.IsEqual(Hero->GetTeam());
	if (bSameTeam)
	{
		spawningVariables.creepCount--;
		return (creepArray.Num() > 0 ? creepArray.Pop() : nullptr);
	}
	return nullptr;
}
