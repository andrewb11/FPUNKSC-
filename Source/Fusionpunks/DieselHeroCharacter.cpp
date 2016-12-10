// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Fusionpunks.h"
#include "RankUIWidget.h"
#include "FusionpunksGameState.h"
#include "DieselHeroCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AFusionpunksCharacter

ADieselHeroCharacter::ADieselHeroCharacter()
{
	maxHealth = 250;
	currentHealth = maxHealth;
	basicAttackDamage = 10;

	respawnTime = 1;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName::FName("HammerSocket"));

	//delete once animation is fixed 
	HammerCollisionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxTrigger"));
	HammerCollisionTrigger->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepRelativeTransform);

	Tags.Add(TEXT("Diesel"));
	team = FName::FName(TEXT("Diesel"));
}

void ADieselHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UIWidgetClass && !ActorHasTag(TEXT("AI")))
	{
		APlayerController* controller = Cast<APlayerController>(GetController());
		if (controller)
		{
			UIWidget = CreateWidget<URankUIWidget>(controller, UIWidgetClass);
			UIWidget->SetOwningCharacter(this);
			UIWidget->AddToPlayerScreen();
		}
	}
	HammerCollisionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADieselHeroCharacter::OnHammerComponentOverlap);
	//WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &ADieselHeroCharacter::OnHammerComponentOverlap); //Restore Once animatino is fixed
}

void ADieselHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADieselHeroCharacter::InitializeHUD()
{
	Super::InitializeHUD();

	if (UIWidgetClass && !ActorHasTag(TEXT("AI")))
	{
		APlayerController* controller = Cast<APlayerController>(GetController());
		if (controller)
		{
			UIWidget = CreateWidget<URankUIWidget>(controller, UIWidgetClass);
			UIWidget->SetOwningCharacter(this);
			UIWidget->AddToPlayerScreen();
		}
		else
		{
			UIWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ADieselHeroCharacter::LevelUp()
{
	if (currentLevel < maxLevel)
	{
		Super::LevelUp();

		//notify GameState we leveled up
		if (GetWorld())
		{
			AFusionpunksGameState* gameState = Cast<AFusionpunksGameState>(GetWorld()->GetGameState());
			if (gameState)
			{
				gameState->DieselLevelUp();
			}	
		}
	}
}

void ADieselHeroCharacter::OnHammerComponentOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//if creep and not on our team OR is an AI enemy
	if (bIsAttacking && !OtherActor->ActorHasTag(team))
	{
		//randomize damage by 100
		float random = FMath::RandRange(0, 100);
		OtherActor->TakeDamage(basicAttackDamage + random, FDamageEvent::FDamageEvent(), GetController(), this);
	}
}

void ADieselHeroCharacter::MeleeAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Using Diesel Melee Attack!"));
	Super::MeleeAttack();
	//stop character movement

	if (!bIsAttacking && !bIsThrowingCreep)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
		
		bIsAttacking = true;

		PlayAnimMontage(MeleeAttackOne);
		launchForce = launchForceOne;
		combo++;
	}
	else
	{
		bChainAttack = true;
	}
}

void ADieselHeroCharacter::ChainCombo()
{
	//UE_LOG(LogTemp, Warning, TEXT("ChainCombo!!!"));
	if (bChainAttack)
	{
		bChainAttack = false;

		UAnimMontage* AnimMontageToPlay = nullptr;

		//play animation based on combo
		switch (combo)
		{
		case 0:
		{
			AnimMontageToPlay = MeleeAttackOne;
			launchForce = launchForceOne;
			combo++;
		}break;

		case 1:
		{
			AnimMontageToPlay = MeleeAttackTwo;
			launchForce = launchForceTwo;
			combo++;
		}break;

		case 2:
		{
			combo = 0;
			bChainAttack = false;
			bIsAttacking = false;
		}break;
		}

		if (AnimMontageToPlay != NULL)
		{
			PlayAnimMontage(AnimMontageToPlay);
		}
	}
}

void ADieselHeroCharacter::OnDeath()
{
	WeaponMesh->SetVisibility(false);
}

void ADieselHeroCharacter::OnRespawn()
{
	WeaponMesh->SetVisibility(true);
}

void ADieselHeroCharacter::PossessTurret()
{
	Super::PossessTurret();

	if (GetWorld()->GetName().Contains("TestLevel"))
	{
		UIWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

