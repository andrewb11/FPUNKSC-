// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "FusionpunksGameState.h"
#include "BaseReactor.h"


// Sets default values
ABaseReactor::ABaseReactor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ReactorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorDestructibleMesh"));
	RootComponent = ReactorMesh;
}

// Called when the game starts or when spawned
void ABaseReactor::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = maxHealth;
}

// Called every frame
void ABaseReactor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}


float ABaseReactor::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	currentHealth -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Base took %f damage."), DamageAmount);
	if (currentHealth <= 0)
	{

		isDestroyed = true;

		if (Tags.Contains("CyberBase"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Diesel Wins!"));
			AFusionpunksGameState* gameState = Cast<AFusionpunksGameState>(GetWorld()->GetGameState());
			if (gameState)
			{
				gameState->DieselWins();
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cyber Wins!"));
			AFusionpunksGameState* gameState = Cast<AFusionpunksGameState>(GetWorld()->GetGameState());
			if (gameState)
			{
				gameState->CyberWins();
			}
		}
	}



	return DamageAmount;
}
