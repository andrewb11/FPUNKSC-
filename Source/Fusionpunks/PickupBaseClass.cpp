// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "PickupBaseClass.h"


// Sets default values
APickupBaseClass::APickupBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	RootComponent = TriggerBox;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("CubeMesh");
	CubeMesh->AttachToComponent(TriggerBox, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APickupBaseClass::OnOverlapBegin);
	
}

// Called when the game starts or when spawned
void APickupBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupBaseClass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickupBaseClass::OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AHeroBase::StaticClass()))
	{
		switch (PickupType)
		{
			case EPickupType::Health :
			{
				AHeroBase* hero = Cast<AHeroBase>(OtherActor);
				if (hero)
				{
					hero->RestoreHealthFull();
				}
			}break;
			case EPickupType::TowerBarExperience :
			{
				/*FOutputDeviceNull ar;
				GetWorld()->GetGameState()->CallFunctionByNameWithArguments(TEXT("AddToTowerProgress"), ar, NULL, true);*/
			}break;
			case EPickupType::SpawnCreepArmy :
			{

			}break;
		}
	}
	Destroy();
}

