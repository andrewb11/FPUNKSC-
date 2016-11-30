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

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ADieselHeroCharacter::OnHit);
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

//////////////////////////////////////////////////////////////////////////
// Input
void ADieselHeroCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	Super::SetupPlayerInputComponent(InputComponent);
	// Set up gameplay key bindings
	//InputComponent->BindAction("MeleeAttack", IE_Pressed, this, &ADieselHeroCharacter::MeleeAttack);
}

void ADieselHeroCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
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
	//UE_LOG(LogTemp, Warning, TEXT("Sword Component Hit!"));

	//if creep and not on our team OR is an AI enemy
	if (bIsAttacking && !OtherActor->Tags.Contains(team))
	{
		OtherActor->TakeDamage(basicAttackDamage, FDamageEvent::FDamageEvent(), GetController(), this);
	}
}

//Change to include animations and work like prophets attack but with a hammer 
//void ADieselHeroCharacter::MeleeAttack()
//{
//	//UE_LOG(LogTemp, Display, TEXT("Basic Attack PRESSED"));
//
//	AActor *closestEnemy;
//
//	FCollisionObjectQueryParams obejctQP;
//	obejctQP.AddObjectTypesToQuery(Hero);
//	obejctQP.AddObjectTypesToQuery(Creeps);
//	obejctQP.AddObjectTypesToQuery(DamageableStructures);
//	//Overlap multi by channel as a sphere (for pick ups?)
//	FCollisionQueryParams QueryParameters;
//	QueryParameters.AddIgnoredActor(this);
//	QueryParameters.OwnerTag = TEXT("Player");
//
//	TArray<FOverlapResult> Results;
//	GetWorld()->OverlapMultiByObjectType(Results,
//		GetActorLocation(),
//		FQuat(),
//		obejctQP,
//		FCollisionShape::MakeSphere(300.f),
//		QueryParameters);
//
//	if (Results.Num() == 0) {
//		UE_LOG(LogTemp, Display, TEXT("No Enemies Nearby"));
//	}
//	else
//	{
//		UE_LOG(LogTemp, Display, TEXT("Enemy Found"));
//		closestEnemy = Results[0].GetActor();
//		for (int i = 0; i < Results.Num(); i++)
//		{
//			if (GetDistanceTo(Results[i].GetActor()) <= GetDistanceTo(closestEnemy))
//			{
//				closestEnemy = Results[i].GetActor();
//			}
//
//		}
//		//check if they are on the same team as us or not
//		if (!closestEnemy->ActorHasTag(team))
//		{
//			Attack(closestEnemy);
//		}
//	}
//
//	//UBoolProperty* boolProp = FindField<UBoolProperty>(GetMesh()->GetAnimInstance()->GetClass(), TEXT("MeleeAttacking"));
//	//if (boolProp)
//	//{
//	//	bIsAttacking = true;
//	//	boolProp->SetPropertyValue_InContainer(GetMesh()->GetAnimInstance(), true);
//	//	//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
//	//	GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AHeroBase::StopAttacking, 1.0f, false);
//	//}
//}

void ADieselHeroCharacter::MeleeAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Using Diesel Melee Attack!"));
	Super::MeleeAttack();
	//stop character movement

	if (!bIsAttacking)
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

