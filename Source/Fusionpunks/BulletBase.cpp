// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"
#include "HeroBase.h"
#include "BulletBase.h"


// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BulletMeshComponent");
	bulletMeshComponent->bGenerateOverlapEvents;
	bulletMeshComponent->SetSimulatePhysics(false);
	RootComponent = bulletMeshComponent;

	bulletTrigger = CreateDefaultSubobject<UCapsuleComponent>("BulletTrigger");
	bulletTrigger->AttachToComponent(bulletMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	bulletTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnOverlapBegin);

	projectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	particleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	particleSystemComp->AttachToComponent(bulletMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

	InitialLifeSpan = 5.0f;

	Tags.Add(TEXT("Bullet"));
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	
	/*if(IsValid(GetOwner()))
	{
		projectileMovementComp->SetVelocityInLocalSpace(GetOwner()->GetActorForwardVector() * speed);
	}*/
}

// Called every frame
void ABulletBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );	
}

void ABulletBase::OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(ACreep::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit Creep!"));
		ACreep* creep = Cast<ACreep>(OtherActor);
		if (creep)
		{
			if(!ActorHasTag(creep->GetTeam()))
			{
				OtherActor->TakeDamage(damage, FDamageEvent::FDamageEvent(), GetOwner()->GetInstigatorController(), GetOwner());
				Destroy();
			}
		}
	}
	
	/*AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		if (!OtherActor->Tags.Contains(hero->GetTeam()))
		{
			OtherActor->TakeDamage(damage, FDamageEvent::FDamageEvent(), hero->GetController(), hero);
			return; 
		}
	}*/
	
}

void ABulletBase::Fire(float Speed, FVector Direction)
{
	//UE_LOG(LogTemp, Warning, TEXT("Direction is: %s"), Direction.ToString());
	//projectileMovementComp->SetVelocityInLocalSpace(Direction * Speed);
	//projectileMovementComp->Velocity = Direction * Speed;
	projectileMovementComp->SetVelocityInLocalSpace(Speed * Direction);
	projectileMovementComp->UpdateComponentVelocity();
}

