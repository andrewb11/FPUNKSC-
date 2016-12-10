// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"
#include "HeroBase.h"
#include "Ability_ForcePush.h"


AAbility_ForcePush::AAbility_ForcePush()
{
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	RootComponent = ParticleSystem;
}

void AAbility_ForcePush::BeginPlay()
{
	Super::BeginPlay();
}

bool AAbility_ForcePush::Ability()
{
	//always move this to the actors location instead of deleting and respawning
	SetActorLocation(GetOwner()->GetActorLocation());

	ParticleSystem->Activate();

	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		UBoolProperty* boolProp = FindField<UBoolProperty>(hero->GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsForcePush"));
		if (boolProp)
		{
			boolProp->SetPropertyValue_InContainer(hero->GetMesh()->GetAnimInstance(), true);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Using FORCE PUSH!"));
	FCollisionObjectQueryParams obejctQP;

	obejctQP.AddObjectTypesToQuery(Hero);
	obejctQP.AddObjectTypesToQuery(Creeps);

	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(GetOwner());
	QueryParameters.OwnerTag = TEXT("Player");

	TArray<FOverlapResult> Results;
	GetWorld()->OverlapMultiByObjectType(Results,GetActorLocation(),FQuat(),obejctQP,FCollisionShape::MakeSphere(PushRadius), QueryParameters);

	//Spawn Effect / Particle System

	if (Results.Num() > 0)
	{
		ACharacter* owner = Cast<ACharacter>(GetOwner());
		if (owner)
		{
			for (int i = 0; i < Results.Num(); i++)
			{
				if (Results[i].GetActor()->IsA(ACreep::StaticClass()))
				{
					ACreep* enemyCreep = Cast<ACreep>(Results[i].GetActor());
					if (enemyCreep)
					{
						if (!owner->Tags.Contains(enemyCreep->GetTeam()))
						{
							enemyCreep->TakeDamage(damage, FDamageEvent::FDamageEvent(), owner->GetController(), owner);
							enemyCreep->Stun(1.0f);
							enemyCreep->GetCharacterMovement()->Velocity = FVector::ZeroVector;

							FVector dir = enemyCreep->GetActorLocation() - owner->GetActorLocation();
							dir.Normalize();
							dir *= PushForce;
							dir.Z += 500.0f;
							dir += enemyCreep->GetCharacterMovement()->Velocity;

							UE_LOG(LogTemp, Warning, TEXT("Adding force to enemy creep: %f"), PushForce);

							enemyCreep->GetCharacterMovement()->Launch(dir);
							//enemyCreep->GetCharacterMovement()->UpdateComponentVelocity();

							continue;
						}
					}
				}
				
				else if (Results[i].GetActor()->IsA(AHeroBase::StaticClass())) 
				{
					AHeroBase* enemyHero = Cast<AHeroBase>(GetOwner());
					if (enemyHero)
					{
						enemyHero->TakeDamage(damage, FDamageEvent::FDamageEvent(), owner->GetController(), owner);
						FVector dir = enemyHero->GetActorLocation() - owner->GetActorLocation();
						dir.Normalize();
						dir *= PushForce;
						//enemyCreep->GetMesh()->AddForceAtLocation(dir, enemyCreep->GetActorLocation());
						//enemyHero->GetCharacterMovement()->AddImpulse(owner->GetActorLocation(), true);
						enemyHero->GetCharacterMovement()->AddForce(dir);
						continue;
					}
				}

			}
		}
	}
	

	return true; 
}