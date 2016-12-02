// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "BaseDoor.h"

ABaseDoor::ABaseDoor()
{

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	destructMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructComp"));
	RootComponent = mesh;
	destructMesh->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform);
	currentHealth = maxHealth;
}

void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = maxHealth;
}

float ABaseDoor::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	currentHealth -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Base took %f damage."), DamageAmount);

	if (currentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Base Destroyed"), DamageAmount);
		mesh->SetVisibility(false);
		destructMesh->SetSimulatePhysics(true);
		//SetActorEnableCollision(false);
		mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}



	return DamageAmount;
}