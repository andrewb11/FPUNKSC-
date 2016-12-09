// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Fusionpunks.h"
#include "ChainLightning.h"
#include  "Creep.h"
#include "FusionpunksGameState.h"
#include "ProphetUIWidget.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "CyberHeroCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AMobaCharacter

ACyberHeroCharacter::ACyberHeroCharacter()
{
	SwordMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordSkeletalMeshComp"));
	SwordMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Sword_socket"));

	maxHealth = 250;
	currentHealth = maxHealth;

	Tags.Add(TEXT("Cyber"));
	team = FName::FName(TEXT("Cyber"));
}

void ACyberHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	SwordMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ACyberHeroCharacter::OnSwordComponentOverlap);

	if (UIWidgetClass && !ActorHasTag(TEXT("AI")))
	{
		APlayerController* controller = Cast<APlayerController>(GetController());
		if (controller)
		{
			UIWidget = CreateWidget<UProphetUIWidget>(controller, UIWidgetClass);
			UIWidget->SetOwningCharacter(this);
			UIWidget->AddToPlayerScreen();
		}		
	}
}

void ACyberHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACyberHeroCharacter::InitializeHUD()
{
	Super::InitializeHUD();

	if (UIWidgetClass && !ActorHasTag(TEXT("AI")))
	{
		APlayerController* controller = Cast<APlayerController>(GetController());
		if (controller)
		{
			if (!UIWidget)
			{
				UIWidget = CreateWidget<UProphetUIWidget>(controller, UIWidgetClass);
				UIWidget->SetOwningCharacter(this);
				UIWidget->AddToPlayerScreen();
			}
			else
			{
				UIWidget->SetVisibility(ESlateVisibility::Visible);
			}
			
		}
	}
}

void ACyberHeroCharacter::MeleeAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Using Cyber Melee Attack!"));
	Super::MeleeAttack();
	
	if (!bIsAttacking)
	{
		GetCharacterMovement()->MaxWalkSpeed *= 0.25f;

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


void ACyberHeroCharacter::ChainCombo()
{
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
			AnimMontageToPlay = MeleeAttackThree;
			launchForce = launchForceThree;
			combo++;
		}break;
		case 3:
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


void ACyberHeroCharacter::AddAffectedActor(AActor* enemy)
{
	affectedActors.Add(enemy);
}

bool ACyberHeroCharacter::IsAffected(AActor* enemy)
{
	return affectedActors.Contains(enemy);
}

void ACyberHeroCharacter::UnHighlightTarget(AActor* enemy)
{
	if (enemy->IsA(ACharacter::StaticClass()))
	{
		Cast<ACharacter>(enemy)->GetMesh()->SetRenderCustomDepth(false);
	}
}

void ACyberHeroCharacter::HighlightTarget(AActor* enemy, TArray<FOverlapResult> enemies)
{
	if (enemy->IsA(ACharacter::StaticClass()))
	{
		Cast<ACharacter>(enemy)->GetMesh()->SetRenderCustomDepth(true);
		Cast<ACharacter>(enemy)->GetMesh()->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;
	}

	for (int i = 0; i < enemies.Num(); i++)
	{
		if (enemy != enemies[i].GetActor() && enemies[i].GetActor()->IsA(ACharacter::StaticClass()))
		{
			Cast<ACharacter>(enemies[i].GetActor())->GetMesh()->SetRenderCustomDepth(false);
		}
	}
}

void ACyberHeroCharacter::UnHighlightAll(TArray<FOverlapResult> enemies)
{
	for (int i = 0; i < enemies.Num(); i++)
	{
		if (enemies[i].GetActor() != nullptr)
		{
			if (enemies[i].GetActor()->IsA(ACharacter::StaticClass()))
			{
				Cast<ACharacter>(enemies[i].GetActor())->GetMesh()->SetRenderCustomDepth(false);
			}
		}
	}
}

void ACyberHeroCharacter::LevelUp()
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
				gameState->CyberLevelUp();
			}
		}
	}
}

void ACyberHeroCharacter::OnSwordComponentOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (bIsAttacking && !OtherActor->ActorHasTag(team))
	{
		//randomize damage by 100
		float random = FMath::RandRange(0, 100);
		OtherActor->TakeDamage(basicAttackDamage + random, FDamageEvent::FDamageEvent(), GetController(), this);
	}
}

void ACyberHeroCharacter::OnDeath()
{
	SwordMeshComp->SetVisibility(false);
}

void ACyberHeroCharacter::OnRespawn()
{
	SwordMeshComp->SetVisibility(true);
}

void ACyberHeroCharacter::PossessTurret()
{
	Super::PossessTurret();

	UIWidget->SetVisibility(ESlateVisibility::Hidden);
}
