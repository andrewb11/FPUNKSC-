// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "BulletBase.h"
#include "HeroBase.h"
#include "Turret.h"


// Sets default values
ATurret::ATurret()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set our turn rates for input
	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate

	TurretBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretBaseMesh");
	TurretBaseMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TurretGunMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretGunMesh");
	TurretGunMesh->AttachToComponent(TurretBaseMesh, FAttachmentTransformRules::KeepRelativeTransform);

	BulletSpawnPoint1 = CreateDefaultSubobject<USceneComponent>("Bullet1SpawnPoint");
	BulletSpawnPoint1->AttachToComponent(TurretGunMesh, FAttachmentTransformRules::KeepRelativeTransform);

	BulletSpawnPoint2 = CreateDefaultSubobject<USceneComponent>("Bullet2SpawnPoint");
	BulletSpawnPoint2->AttachToComponent(TurretGunMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ChargeBeamSpawnPoint1 = CreateDefaultSubobject<USceneComponent>("ChargeBeam1SpawnPoint");
	ChargeBeamSpawnPoint1->AttachToComponent(TurretGunMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ChargeBeamSpawnPoint2 = CreateDefaultSubobject<USceneComponent>("ChargeBeam2SpawnPoint");
	ChargeBeamSpawnPoint2->AttachToComponent(TurretGunMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HeroInRangeRadius = CreateDefaultSubobject<USphereComponent>("TurretRadius");
	HeroInRangeRadius->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HeroInRangeRadius->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnOverlapBegin);
	HeroInRangeRadius->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnOverlapEnd);

	TurretPillarMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretPillarMesh");
	TurretPillarMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TurretCamera = CreateDefaultSubobject<UCameraComponent>("TurretCamera");
	TurretCamera->AttachToComponent(TurretGunMesh, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurret::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bIsCharging)
	{
		FVector vector(1, 1, 1);
		//Implement Limit
		if (BeamOne)
		{
			UE_LOG(LogTemp, Warning, TEXT("Scaling Bullet 1!"));
			//BeamOne->GetTransform().MultiplyScale3D(vector * ChargeBeamSizeScaleRate * DeltaTime);
			BeamOne->SetActorScale3D(BeamOne->GetActorScale3D() + ChargeBeamSizeScaleRate * DeltaTime);
			if (BeamOne->GetActorScale3D().Size() > MaxScale.Size())
			{
				BeamOne->SetActorScale3D(FVector::FVector(1, 1, 1) * MaxScale);
			}
		}
		if (BeamTwo)
		{
			//BeamTwo->GetTransform().MultiplyScale3D(vector * ChargeBeamSizeScaleRate * DeltaTime);
			BeamTwo->SetActorScale3D(BeamTwo->GetActorScale3D() + ChargeBeamSizeScaleRate * DeltaTime);
			if (BeamTwo->GetActorScale3D().Size() > MaxScale.Size())
			{
				BeamTwo->SetActorScale3D(FVector::FVector(1, 1, 1) * MaxScale);
			}
		}
	}
}

// Called to bind functionality to input
void ATurret::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis("TurnRate", this, &ATurret::TurnAtRate);
	//InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//InputComponent->BindAxis("LookUpRate", this, &ATurret::LookUpAtRate);

	InputComponent->BindAxis("LookUpTurretGun", this, &ATurret::LookUpGun);
	InputComponent->BindAxis("TurnTurretGun", this, &ATurret::RotateBase);
	
	InputComponent->BindAction("PossessTurret", IE_Pressed, this, &ATurret::ReturnToHero);
	InputComponent->BindAction("FireTurret", IE_Pressed, this, &ATurret::FireBullet);

	//InputComponent->BindAction("ChargeBeam", IE_Pressed, this, &ATurret::ChargeBeam);
	//InputComponent->BindAction("ChargeBeam", IE_Released, this, &ATurret::ChargeBeam);
}

void ATurret::LookUpGun(float Rate)
{
	TurretGunMesh->AddRelativeRotation(FRotator::FRotator(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds(), 0, 0));
}

void ATurret::RotateBase(float Rate)
{
	TurretBaseMesh->AddRelativeRotation(FRotator::FRotator(0, Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds(), 0));
}

void ATurret::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATurret::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATurret::FireBullet()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire Turret Bullet!"));

	if (ShootSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);
	}


	//Spawn 2 bullets and fire them
	if (BulletToSpawn)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (owningHero)
		{
			spawnParams.Owner = owningHero;
			spawnParams.Instigator = owningHero;
		}

		////spawn bullet 1 and 2
		FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		ViewportSize *= 0.5f;

		FVector Location1 = BulletSpawnPoint1->GetComponentLocation();
		FVector Direction1 = FVector::ForwardVector;

		FVector Location2 = BulletSpawnPoint2->GetComponentLocation();
		FVector Direction2 = FVector::ForwardVector;

		if (owningHero)
		{
			APlayerController* controller = Cast<APlayerController>(owningHero->GetController());
			if (controller)
			{
				UGameplayStatics::DeprojectScreenToWorld(controller, ViewportSize, Location1, Direction1);
				UGameplayStatics::DeprojectScreenToWorld(controller, ViewportSize, Location2, Direction2);
			}
		}
		FVector newDirection1 = Location1 - BulletSpawnPoint1->GetComponentLocation();
		newDirection1.Normalize();
		FVector newDirection2 = Location2 - BulletSpawnPoint2->GetComponentLocation();
		newDirection2.Normalize();

		ABulletBase* bullet1 = GetWorld()->SpawnActor<ABulletBase>(BulletToSpawn, BulletSpawnPoint1->GetComponentLocation(), BulletSpawnPoint1->GetComponentRotation(), spawnParams);
		if (bullet1)
		{
			bullet1->Fire(bulletSpeed, Direction1);
		}


		ABulletBase* bullet2 = GetWorld()->SpawnActor<ABulletBase>(BulletToSpawn, BulletSpawnPoint2->GetComponentLocation(), BulletSpawnPoint2->GetComponentRotation(), spawnParams);
		if (bullet2)
		{
			bullet2->Fire(bulletSpeed, Direction2);
		}
	}
			
}

void ATurret::OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AHeroBase::StaticClass()))
	{
		AHeroBase* hero = Cast<AHeroBase>(OtherActor);
		if (hero)
		{
			hero->bTurretCloseBy = true;
			hero->SetNearbyTurret(this);
			hero->ShowUseTowerWidget();
		}
	}
}

void ATurret::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AHeroBase::StaticClass()))
	{
		AHeroBase* hero = Cast<AHeroBase>(OtherActor);
		if (hero)
		{
			hero->bTurretCloseBy = false;
			hero->SetNearbyTurret(nullptr);
			hero->HideUseTowerWidget();
		}
	}
}

void ATurret::ReturnToHero()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying To Possess Hero!"));
	if (owningHero)
	{
		//remove turret widget
		owningHero->bIsHidden = false;
		owningHero->SetActorHiddenInGame(false);

		APlayerController* controller = Cast<APlayerController>(GetController());
		if (controller)
		{
			controller->Possess(owningHero);
			controller->SetViewTargetWithBlend(owningHero);
			//Restore HUD
			owningHero->InitializeHUD();
		}
		if (TurretWidget)
		{
			TurretWidget->RemoveFromParent();
		}
	}
}

void ATurret::ChargeBeam()
{
	//swap our states
	bIsCharging = !bIsCharging;
	//check state
	if (bIsCharging)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Right Mouse Button Pressed!"));
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (owningHero)
		{
			spawnParams.Owner = owningHero;
			spawnParams.Instigator = owningHero;
		}

		BeamOne = GetWorld()->SpawnActor<ABulletBase>(ChargeBeamBullet, ChargeBeamSpawnPoint1->GetComponentLocation(), ChargeBeamSpawnPoint1->GetComponentRotation(), spawnParams);
		BeamOne->AttachToComponent(ChargeBeamSpawnPoint1, FAttachmentTransformRules::KeepWorldTransform);
		
		BeamTwo = GetWorld()->SpawnActor<ABulletBase>(ChargeBeamBullet, ChargeBeamSpawnPoint2->GetComponentLocation(), ChargeBeamSpawnPoint2->GetComponentRotation(), spawnParams);
		BeamTwo->AttachToComponent(ChargeBeamSpawnPoint2, FAttachmentTransformRules::KeepWorldTransform);
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Right Mouse Button Released!"));
	if (BeamOne)
	{
		BeamOne->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		BeamOne->Fire(ChargeBeamBulletSpeed, ChargeBeamSpawnPoint1->GetForwardVector());
	}
	if (BeamTwo)
	{
		BeamTwo->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		BeamTwo->Fire(ChargeBeamBulletSpeed, ChargeBeamSpawnPoint2->GetForwardVector());
	}

}

void ATurret::InitTurretWidget()
{
	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (playerController && TurretWidgetClass)
	{
		TurretWidget = CreateWidget<UUserWidget>(playerController, TurretWidgetClass);
		TurretWidget->AddToPlayerScreen();
	}
}



