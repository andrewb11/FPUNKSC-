// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "FusionpunksGameState.h"
#include "FloatingDamageWidget.h"
#include "HeroBase.h"
#include "BaseReactor.h"


// Sets default values
ABaseReactor::ABaseReactor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//col = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCol"));
	ReactorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorDestructibleMesh"));
	RootComponent = ReactorMesh;
	//ReactorMesh->AttachToComponent(col, FAttachmentTransformRules::KeepRelativeTransform);

	structureRadius = CreateDefaultSubobject<USphereComponent>(TEXT("StructureRadius"));
	structureRadius->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	structureRadius->SetSphereRadius(300);
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


	if (DamageCauser && DamageCauser->IsA(AHeroBase::StaticClass()) && !DamageCauser->ActorHasTag("AI") && FloatingDamageWidgetClass)
	{

		AHeroBase* enemy = Cast<AHeroBase>(DamageCauser);
		APlayerController *playerController = nullptr;
		if (enemy)
			playerController = Cast<APlayerController>(enemy->GetController());
		if (playerController)
		{
			UFloatingDamageWidget* floatingDamageWidget = CreateWidget<UFloatingDamageWidget>(playerController, FloatingDamageWidgetClass);
			//floatingDamageWidget->SetAlignmentInViewport(FVector2D::FVector2D(0.5f, 0.5f));
			floatingDamageWidget->SetIncDamage(DamageAmount);
			floatingDamageWidget->SetOwningActor(this);
			floatingDamageWidget->AddToPlayerScreen();
		}
	}


	if (currentHealth <= 0)
	{

		isDestroyed = true;
		enemyHero->HideStructureHB(this);
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
