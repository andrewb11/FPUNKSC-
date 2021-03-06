// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"
#include "HeroBase.h"
#include "Explosion.h"


// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionTrigger"));
	RootComponent = ExplosionTrigger;

	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
	ExplosionEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ExplosionForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionForceComponent"));
	ExplosionForceComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ExplosionTrigger->OnComponentBeginOverlap.AddDynamic(this, &AExplosion::OnOverlapBegin);
	InitialLifeSpan = 2.0f;

	team = FName::FName("Diesel");
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();

	if (bFriendlyFire)
	{
		team = FName::FName("Neutral");
	}

	//UE_LOG(LogTemp, Warning, TEXT("Spawning Explosion!"));

	if (ExplosionSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}

	ExplosionEffect->Activate(true);
	ExplosionForceComponent->FireImpulse();
}

// Called every frame
void AExplosion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionForceComponent->Radius, 8, FColor::Red);
}

void AExplosion::OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult &SweepResult)
{
	if (!OtherActor->ActorHasTag(team))
	{
		if (IsValid(GetOwner()))
		{
			AHeroBase* hero = Cast<AHeroBase>(GetOwner());
			if (hero)
			{
				OtherActor->TakeDamage(Damage, FRadialDamageEvent::FDamageEvent(), hero->GetController(), hero);
			}
		}	
		else
		{
			OtherActor->TakeDamage(Damage, FRadialDamageEvent::FDamageEvent(), NULL, NULL);
		}
	}
}
