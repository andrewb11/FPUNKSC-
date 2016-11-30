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

	//if (skillSelected) 
	//{
	//	UpdateTarget();
	//}
}

void ACyberHeroCharacter::MeleeAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Using Cyber Melee Attack!"));
	Super::MeleeAttack();
	
	if (!bIsAttacking)
	{
		//stop character movement
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


void ACyberHeroCharacter::ChainCombo()
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



//////////////////////////////////////////////////////////////////////////
// Input

void ACyberHeroCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	Super::SetupPlayerInputComponent(InputComponent);
	check(InputComponent);

	//InputComponent->BindAction("MeleeAttack", IE_Pressed, this, &ACyberHeroCharacter::MeleeAttack);
	//InputComponent->BindAction("Skill1", IE_Pressed, this, &ACyberHeroCharacter::OnSkillPressed);
}


//void ACyberHeroCharacter::DetermineClickEvent()
//{
//	if (skillSelected)
//	{
//		AActor* currentTarget = UpdateTarget();
//		if (currentTarget != NULL)
//		{
//			UseSkill(currentTarget);
//			UnHighlightTarget(currentTarget);
//			skillSelected = false;
//		}
//	}
//}


//AActor* ACyberHeroCharacter::UpdateTarget()
//{
//	//UE_LOG(LogTemp, Display, TEXT("Skill Used"));
//	if (oldTargetResults.Num() > 0)
//	{
//		UnHighlightAll(oldTargetResults);
//	}
//
//	AActor *closestEnemy;
//	FCollisionObjectQueryParams obejctQP;
//	obejctQP.AddObjectTypesToQuery(Creeps);
//	obejctQP.AddObjectTypesToQuery(Hero);
//	//obejctQP.AddObjectTypesToQuery()
//	//Overlap multi by channel as a sphere (for pick ups?)
//
//	FCollisionQueryParams QueryParameters;
//	QueryParameters.AddIgnoredActor(this);
//	QueryParameters.OwnerTag = TEXT("Player");	
//	GetWorld()->OverlapMultiByObjectType(skillTargetResults,
//		GetActorLocation(),
//		FQuat(),
//		obejctQP,
//		FCollisionShape::MakeSphere(750),
//		QueryParameters);
//	oldTargetResults = skillTargetResults;
//	
//	if (skillTargetResults.Num() == 0)
//	{
//		UE_LOG(LogTemp, Display, TEXT("No Units Nearby"));
//		return NULL;
//	}
//	else
//	{
//		UE_LOG(LogTemp, Display, TEXT("Unit Found"));
//
//		TArray<AActor*> enemies;
//		for (int i = 0; i < skillTargetResults.Num(); i++)
//		{
//			if (!skillTargetResults[i].GetActor()->ActorHasTag("Cyber"))
//			{
//				enemies.Add(skillTargetResults[i].GetActor());
//			}
//		}
//
//		if (enemies.Num() > 0)
//		{
//			closestEnemy = enemies[0];
//
//			for (int i = 0; i < enemies.Num(); i++)
//			{
//				if (GetDistanceTo(enemies[i]) <= GetDistanceTo(closestEnemy))
//				{
//					closestEnemy = enemies[i];
//				}
//			}
//
//			HighlightTarget(closestEnemy, skillTargetResults);
//			return closestEnemy;
//		}
//			UE_LOG(LogTemp, Display, TEXT("No Enemies Nearby"));
//			return NULL;
//	}
//}

//void ACyberHeroCharacter::UseAbility1()
//{
//
//	if (!skillSelected)
//	{
//		skillSelected = true;
//	}
//	else 
//	{
//		skillSelected = false;
//		AActor* currentTarget = UpdateTarget();
//
//		if (currentTarget != NULL)
//		{
//			UnHighlightTarget(currentTarget);
//		}
//	}
//}
//
//void ACyberHeroCharacter::UseSkill(AActor* enemy)
//{
//
//	FActorSpawnParameters spawnParams;
//	spawnParams.Owner = this; 
//	spawnParams.Instigator = this;
//
//	FVector spawnLoc;
//	spawnLoc = GetActorLocation();
//	spawnLoc.Z = spawnLoc.Z + 200;
//   	AChainLightning* lightning =  GetWorld()->SpawnActor
//		<AChainLightning>(chainLightningAbility,
//		spawnLoc,
//		FRotator::ZeroRotator, spawnParams);
//	lightning->AddAffectedActor(enemy);
//	lightning->SetBeamPoints(Cast<AActor>(this), enemy);
//	lightning->Use();
//
//}

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
	//UE_LOG(LogTemp, Warning, TEXT("Sword Component Hit!"));

	//if creep and not on our team OR is an AI enemy
	if (bIsAttacking && !OtherActor->Tags.Contains(team))
	{
		OtherActor->TakeDamage(basicAttackDamage, FDamageEvent::FDamageEvent(), GetController(), this);
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
