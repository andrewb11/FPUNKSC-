// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "CreepHealthbarWidget.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "FloatingDamageWidget.h"
#include "TowerBase.h"
#include "FusionpunksGameState.h"
#include "CreepAIController.h"
#include "Explosion.h"
#include "HeroBase.h"
#include "Creep.h"


ACreep::ACreep()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = true;

	RootComponent = GetCapsuleComponent();

	const ConstructorHelpers::FObjectFinder<UClass>
		CreepHealthBarFinder(TEXT("/Game/UI/CreepHealthBarWidget_BP.CreepHealthBarWidget_BP_C"));
	if (CreepHealthBarFinder.Object != NULL)
	{
		CreepHealthBarWidgetClass = Cast<UClass>(CreepHealthBarFinder.Object);
		widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
		widgetComponent->SetWidgetClass(CreepHealthBarWidgetClass);
		widgetComponent->SetSimulatePhysics(false);
		widgetComponent->bGenerateOverlapEvents = false;
		widgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	const ConstructorHelpers::FObjectFinder<UClass>
		FloatingDamageWidgetFinder(TEXT("/Game/UI/FloatingDamageWidget_BP.FloatingDamageWidget_BP_C"));
	if (FloatingDamageWidgetFinder.Object != NULL)
	{
		FloatingDamageWidgetClass = Cast<UClass>(FloatingDamageWidgetFinder.Object);
	}

	agroRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroRadius"));
	agroRadiusSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	FlameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlameMesh"));
	FlameMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName::FName("BE_Lowerbody"));

	//AIControllerClass = ACreepAIController::StaticClass();
	GetMesh()->bReceivesDecals = false; 
	GetCapsuleComponent()->SetCanEverAffectNavigation(true);


	//NOTE::BRENDON - Will have to change current level based on players level when spawned from a controlled camp 
	//i.e. Diesel hero is level 9 -> creep should also be level 9 
	currentLevel = 1;
	maxLevel = 10;
	maxHealth = 10;
	meleeAttackRange = 200.0f;

	patrolRadius = 2000.0f;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	BaseTurnRate = 45.f;
	distanceFromCamp = 0;

	meleeAttackCooldown = 2.0f;
	meleeAttackCooldownTimer = -0.1f;
	bCanMeleeAttack = true; 
	bBelongsToCamp = false;
	Tags.Add(TEXT("Creep"));
}

void ACreep::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = maxHealth;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACreep::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACreep::OnOverlapEnd);

	if (GetWorld())
	{
		ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
		if (AiController)
		{
			//AiController = Cast<ACreepAIController>(GetWorld()->SpawnActor<ACreepAIController>(AIControllerClass));
			//AiController->Possess(this);
			AiController->GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), this);
			AiController->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			//AiController->GetBlackboardComponent()->SetValueAsBool(TEXT("belongsToCamp"), bBelongsToCamp);
		}
		
		/*if (creepCampHome != nullptr)
		{
			AiController->GetBlackboardComponent()->SetValueAsObject(TEXT("CreepCampHome"), creepCampHome);
		}*/
		localPlayer = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	}

	//AiController = Cast<ACreepAIController>(GetController());
	/*NOTE::BRENDON -> Creep camp should set this to true when it creates them. Set to false incase we want to spawn
	them from the player or in another situation*/
	//blackboardComponent->SetValueAsBool(TEXT("belongsToCamp"), false);
	
	if (Cast<UCreepHealthbarWidget>(widgetComponent->GetUserWidgetObject()))
	{
		UCreepHealthbarWidget* thisCreepHealthBarWidget = Cast<UCreepHealthbarWidget>(widgetComponent->GetUserWidgetObject());
		thisCreepHealthBarWidget->SetOwningCreep(this);
	}
}

void ACreep::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (localPlayer)
	{
		UCameraComponent* cameraComponent = Cast<UCameraComponent>(localPlayer->GetComponentByClass(UCameraComponent::StaticClass()));
		widgetCompRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), cameraComponent->GetComponentLocation());
		widgetCompRotation.Pitch = 0;
		widgetComponent->SetWorldRotation(widgetCompRotation);
	}

	if (meleeAttackCooldownTimer >= 0)
	{
		meleeAttackCooldownTimer -= DeltaTime;
		bCanMeleeAttack = meleeAttackCooldownTimer <= 0;
	}
	distanceFromCamp = 0;
	if (creepCampHome)
	{
		distanceFromCamp = (GetActorLocation() - creepCampHome->GetActorLocation()).Size();

		if (distanceFromCamp > chaseDistance)
		{
			EnemyTarget = nullptr;
			ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
			if (AiController)
			{
				AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", nullptr);
			}
			//SetToWalk();
		}
	}

	if (!bBelongsToCamp && playerToFollow)
	{
		ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
		if (AiController)
		{
			AiController->GetBlackboardComponent()->SetValueAsVector("FormationPosition", playerToFollow->GetSlotPosition(slotAssignment));
		}	
	}
}

void ACreep::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ACreep::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACreep::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ACreep::TurnAtRate);
}

float ACreep::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (currentHealth > 0)
	{
		currentHealth -= Damage;
		//Play Take Damage Aniamtion
		UBoolProperty* boolProp = FindField<UBoolProperty>(GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsDamaged?"));
		if (boolProp)
		{
			boolProp->SetPropertyValue_InContainer(GetMesh()->GetAnimInstance(), true);
			//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
			GetWorld()->GetTimerManager().SetTimer(takeDamageTimerHandle, this, &ACreep::StopTakingDamageAnim, 0.2f, false);
		}
		//check of creep has an enemy target
		ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
		if (AiController)
		{
			if (AiController->GetBlackboardComponent()->GetValueAsObject("EnemyTarget") == nullptr && !DamageCauser->ActorHasTag(team))
			{
				AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", DamageCauser);
				AiController->RestartBehaviorTree();
			}
		}

		if (Damage < 10000000 && DamageCauser->IsA(AHeroBase::StaticClass()) && !DamageCauser->ActorHasTag("AI") && FloatingDamageWidgetClass)
		{
			APlayerController* playerController = Cast<APlayerController>(EventInstigator);
			if (playerController)
			{
				UFloatingDamageWidget* floatingDamageWidget = CreateWidget<UFloatingDamageWidget>(playerController, FloatingDamageWidgetClass);
				floatingDamageWidget->SetIncDamage(Damage);
				floatingDamageWidget->SetOwningActor(this);
				floatingDamageWidget->Instigator = DamageCauser;
				floatingDamageWidget->AddToPlayerScreen();
			}
		}

		if (currentHealth <= 0)
		{
			if (GetWorld()->GetName().Contains("TestLevel"))
			{
				FOutputDeviceNull ar;
				CallFunctionByNameWithArguments(TEXT("CreepDead"), ar, NULL, true);
			}
			
			//random to see which dying sound we should play
			if (DyingSounds.Num() > 0 && !DamageCauser->ActorHasTag("AI") && Cast<AHeroBase>(DamageCauser))
			{
				int random = FMath::RandRange(0, DyingSounds.Num() - 1);
				AFusionpunksGameState* gameState = Cast<AFusionpunksGameState>(UGameplayStatics::GetGameState(GetWorld()));
				if (gameState)
				{
					UGameplayStatics::PlaySoundAtLocation(gameState->Players[0], DyingSounds[random], GetActorLocation());
				}
			}

			bIsDead = true;
			GetCapsuleComponent()->bGenerateOverlapEvents = false;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			agroRadiusSphere->bGenerateOverlapEvents = false;
			widgetComponent->SetVisibility(false);

			ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
			if (IsValid(AiController))
			{
				//Unpossess and destroy the controller
				AiController->UnPossess();
				AiController->Destroy();
			}
			if (bSimPhysicsOnDeath)
			{
				GetMesh()->SetSimulatePhysics(true);
			}
			else
			{
				//play death animation
				if (DeathAnimation)
				{
					//if doesn't work use montage instead
					GetMesh()->PlayAnimation(DeathAnimation, false);
				}
			}
			
			FlameMesh->DestroyComponent();
			GetWorld()->GetTimerManager().SetTimer(destroyCreepTimerHandle, this, &ACreep::KillCreep, 5.0f, false);

			if (DamageCauser->IsA(AHeroBase::StaticClass()) && !DamageCauser->Tags.Contains(team))
			{
				AHeroBase* hero = Cast<AHeroBase>(DamageCauser);
				if (hero)
				{
					hero->AddToExperience(XPKillReward);
				}
			}

			if (bBelongsToCamp && creepCampHome != nullptr)
			{
				creepCampHome->RemoveCreep(this);
			}
			else if (!bBelongsToCamp && playerToFollow)
			{
				playerToFollow->RemoveCreepFromArmy(this);
			}
			//this->Destroy();
		}
		return Damage;
	}
	return Damage;
}

void ACreep::LevelUp()
{
	if (currentLevel + 1 <= maxLevel)
	{
		currentLevel++;
		maxHealth += healthIncreasePerLevel;
		attackPower += attackPowerIncreasePerLevel;
		currentHealth = maxHealth;
	}
}

void ACreep::SetCreepCampHome(ACreepCamp* home, bool BelongsToCamp = false)
{
	creepCampHome = home;
	bBelongsToCamp = BelongsToCamp;

	ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
	if (IsValid(AiController))
	{
		AiController->GetBlackboardComponent()->SetValueAsBool(TEXT("belongsToCamp"), bBelongsToCamp);
		AiController->GetBlackboardComponent()->SetValueAsObject(TEXT("CreepCampHome"), creepCampHome);
	}
}

void ACreep::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACreep::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACreep::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
//NOTE::IF CREEP GETS ATTACKED WALKING BACK TO CAMP ATTACK PLAYER 
//function for Trigger Events
UFUNCTION()
void ACreep::OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!OtherActor->ActorHasTag(team) && OtherActor->IsA(AHeroBase::StaticClass()) || !OtherActor->ActorHasTag(team) && OtherActor->IsA(ACreep::StaticClass())
		|| !OtherActor->ActorHasTag(team) && OtherActor->IsA(ATowerBase::StaticClass()))
	{
		ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
		if (AiController)
		{
			if (AiController->GetBlackboardComponent()->GetValueAsObject("EnemyTarget") == nullptr)
			{
				AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", OtherActor);
			}
		}
	}

	if (OtherActor->IsA(ATowerBase::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped with tower!"));
	}

	//ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
	//if (AiController)
	//{
	//	if (AiController->GetBlackboardComponent()->GetValueAsObject("EnemyTarget") == nullptr && !OtherActor->Tags.Contains(team))
	//	{
	//		//UE_LOG(LogTemp, Warning, TEXT("Creep Entered Player Trigger!"));
	//		//SetToRun();

	//		AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", OtherActor);
	//		//AiController->GetBlackboardComponent()->SetValueAsBool("AtTargetPosition", false);
	//		//AiController->GetBlackboardComponent()->SetValueAsBool("hasWaited", true);
	//		//AiController->GetBlackboardComponent()->SetValueAsObject("SelfActor", this);
	//		//AiController->RestartBehaviorTree();
	//	}
	//}
}

//function for Trigger Exit Events
UFUNCTION()
void ACreep::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Creep Exited Trigger!"));
	/*if (EnemyTarget == OtherActor)
	{
		ClearEnemyTarget();
	}*/
}

float ACreep::MeleeAttack()
{
	//if creep hasn't attacked yet
	//NOTE::IMPLEMENT ATTACK ANIMATION PLAYING FOR EACH CREEP TYPE
	if (bCanMeleeAttack)
	{
		UBoolProperty* boolProp = FindField<UBoolProperty>(GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsAttacking?"));
		if (boolProp)
		{
			boolProp->SetPropertyValue_InContainer(GetMesh()->GetAnimInstance(), true);
			//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
			GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &ACreep::StopMeleeAttack, 1.0f, false);
		}
		meleeAttackCooldownTimer = meleeAttackCooldown;
		bCanMeleeAttack = false; 
		//UE_LOG(LogTemp, Warning, TEXT("Creep Attacked for: %f"), attackPower);
		return attackPower;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Creep Attacked for: %f"), 0);
		return 0;
	}
}

void ACreep::StopMeleeAttack()
{
	if (attackTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);

	UBoolProperty* boolProp = FindField<UBoolProperty>(GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsAttacking?"));
	if (boolProp)
	{
		boolProp->SetPropertyValue_InContainer(GetMesh()->GetAnimInstance(), false);
		//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
	}
}

FORCEINLINE void ACreep::SetToWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = patrolMovementSpeed;
}

FORCEINLINE void ACreep::SetToRun()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed; 
}

void ACreep::JoinPlayerArmy(AHeroBase* PlayerToFollow, int SlotAssignment)
{
	//NOTE::Brendon - Might not need reference to player to follow in class
	playerToFollow = PlayerToFollow;
	bBelongsToCamp = false; 

	slotAssignment = SlotAssignment;

	UCharacterMovementComponent* movementComp = GetCharacterMovement();
	//GetCharacterMovement()->MaxWalkSpeed = runSpeed;


	ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
	if (AiController)
	{
		AiController->GetBlackboardComponent()->SetValueAsObject("HeroToFollow", playerToFollow);
		AiController->GetBlackboardComponent()->SetValueAsBool("belongsToCamp", false);
		AiController->GetBlackboardComponent()->SetValueAsVector("SlotPosition", playerToFollow->GetSlotPosition(slotAssignment));
		AiController->RestartBehaviorTree();
		creepCampHome = nullptr;
	}
}

void ACreep::SetEnemyTarget(AActor* enemy)
{
	EnemyTarget = enemy;
	ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
	if (AiController)
	{
		if (EnemyTarget != nullptr)
		{
			AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", EnemyTarget);
			AiController->GetBlackboardComponent()->SetValueAsBool("AtTargetPosition", false);
			AiController->GetBlackboardComponent()->SetValueAsBool("hasWaited", true);
			AiController->RestartBehaviorTree();
		}
		else
		{
			AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", nullptr);
			AiController->RestartBehaviorTree();
		}
	}
}

void ACreep::Stun(float Duration)
{
	ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
	if (AiController)
	{
		AiController->GetBlackboardComponent()->SetValueAsBool("bIsStunned", true);
		AiController->RestartBehaviorTree();
		GetCharacterMovement()->MaxWalkSpeed = 0;
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		
		GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &ACreep::EndStun, 3.0f, false);
	}
}

void ACreep::EndStun()
{
	if (stunTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(stunTimerHandle);

	ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
	if (AiController)
	{
		AiController->GetBlackboardComponent()->SetValueAsBool("bIsStunned", false);
		AiController->RestartBehaviorTree();
		SetToRun();
	}
}

void ACreep::ChangeTeam()
{
	if (Tags.Contains("Cyber"))
	{
		Tags.Remove("Cyber");
		Tags.Add("Diesel");

		team = "Diesel";
	}
	else if (Tags.Contains("Diesel"))
	{
		Tags.Remove("Diesel");
		Tags.Add("Cyber");

		team = "Cyber";
	}
}

void ACreep::AttackLeader()
{
	ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
	if (AiController)
	{
		AActor* Leader = Cast<AActor>(AiController->GetBlackboardComponent()->GetValueAsObject("HeroToFollow"));
		if (Leader)
		{
			AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", Leader);
			AiController->RestartBehaviorTree();
		}	
	}
}

void ACreep::ClearEnemyTarget()
{
	EnemyTarget = nullptr;

	ACreepAIController* AiController = Cast<ACreepAIController>(GetController());
	if (AiController)
	{
		AiController->GetBlackboardComponent()->SetValueAsObject("EnemyTarget", nullptr);
		AiController->RestartBehaviorTree();
	}
}

void ACreep::StopTakingDamageAnim()
{
	if (takeDamageTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(takeDamageTimerHandle);

	UBoolProperty* boolProp = FindField<UBoolProperty>(GetMesh()->GetAnimInstance()->GetClass(), TEXT("IsDamaged?"));
	if (boolProp)
	{
		boolProp->SetPropertyValue_InContainer(GetMesh()->GetAnimInstance(), false);
	}
}

void ACreep::KillCreep()
{
	if(destroyCreepTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(destroyCreepTimerHandle);

	this->Destroy();
}

void ACreep::HideHealthBar()
{
	widgetComponent->SetVisibility(false);
}

void ACreep::StartDetonateTimer(float TimeUntilDetonate)
{
	GetWorld()->GetTimerManager().SetTimer(DetonateTimerHandle, this, &ACreep::Detonate, TimeUntilDetonate, false);
}


void ACreep::Detonate()
{
	UE_LOG(LogTemp, Warning, TEXT("Detonate Creep!"));
	if(DetonateTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(DetonateTimerHandle);

	if (ExplosionClass)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (IsValid(playerToFollow))
		{
			spawnParams.Owner = playerToFollow;
		}

		//Spawn explosion and detatch limbs 
		AExplosion* explosion = GetWorld()->SpawnActor<AExplosion>(ExplosionClass, GetMesh()->GetComponentLocation(), FRotator::ZeroRotator, spawnParams);
	}
}


