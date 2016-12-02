
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AbilityBase.h"
#include "HeroBase.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	Team_Neutral    UMETA(DisplayName = "Neutral"),
	Team_Cyber      UMETA(DisplayName = "Cyber"),
	Team_Diesel	    UMETA(DisplayName = "Diesel")
};

UCLASS(abstract)
class FUSIONPUNKS_API AHeroBase : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
		AActor* AICam;

public:
	AHeroBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator,AActor * DamageCauser) override;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);
	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);
	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		FORCEINLINE float GetPlayerHealthAsDecimal() const { return currentHealth / maxHealth; }

	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		FORCEINLINE float GetCurrentHealth() const { return currentHealth; }

	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		float GetMaxHealth();
	void Attack(AActor* enemy);

	FORCEINLINE float GetMaxHealth() const { return maxHealth; }

	void HealOverTime();
	void Heal(float amount);

	void Slow(float Percentage);

	UPROPERTY(EditDefaultsOnly, Category = Abilities)
		TSubclassOf<class AHealOverTime> healOverTimeClass;
	void StartAttack();

	//bool bIsBusy;

protected:
	void AdjustCameraZoom(float Value);
	void SwapAICamera();

//editable stats in blueprint
protected:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float maxHealth;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int maxLevel;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float basicAttackDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float healthIncreasePerLevel;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float damageIncreasePerLevel;

	UPROPERTY(VisibleAnywhere)
		float currentHealth;
	UPROPERTY(VisibleAnywhere)
		float healthPercent;

	int32 currentLevel;

	UFUNCTION(BlueprintCallable, category = Stats)
		virtual void LevelUp();

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float agroRadius;
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int XPKillReward;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int experienceGoal;

	int currentExperience;
	
	bool bInsideHealingWell = false; 

public:
	void AddToExperience(int experience);
	float GetCurrentExperienceAsDecimal() const { return (float)currentExperience / (float)experienceGoal; }

//CAMP PROGRESS WIDGET
public:
	UFUNCTION(BlueprintCallable, Category = UIFunctions)
		void ShowCampProgress(ACreepCamp* CurrentCamp);

	UFUNCTION(BlueprintCallable, Category = UIFunctions)
		void HideCampProgress();


	void ResetHealth();
	FVector startingLocation;
	bool bIsRespawning = false;

	FORCEINLINE bool IsRespawning() const { return bIsRespawning; }
	FORCEINLINE int32 GetArmySize() const { return CreepArmy.Num(); }
	FORCEINLINE float GetRespawnTime() const { return respawnTime; }
	FORCEINLINE  int32 GetLevel() const { return currentLevel; }
	FORCEINLINE  float GetAttackDamage() const { return basicAttackDamage; }
	FORCEINLINE TArray<class ACreepCamp*> GetCapturedCamps() const { return capturedCamps; }
	FORCEINLINE class HeroStats* GetHeroStats() const { return heroStats; }
	FORCEINLINE void SetIsCapturing(bool status) { isCapturing = status; }
	FORCEINLINE bool IsCapturing() const { return isCapturing; }
	FORCEINLINE  ACreepCamp* GetCampBeingCaptured() const { return campBeingCaptured; }
	FORCEINLINE void SetInsideHealingWell(bool status) {bInsideHealingWell = status;}
	FORCEINLINE bool InsideHealingWell() const { return bInsideHealingWell; }
	FORCEINLINE bool IsCreepAttacking() const { return bCreepIsAttacking; }
	FORCEINLINE bool IsHeroAttacking() const { return bHeroIsAttacking; }
	FORCEINLINE void SetCreepAttacking(bool status) { bCreepIsAttacking = status; }
	FORCEINLINE void SetHeroAttacking(bool status)  { bHeroIsAttacking = status; }
	FORCEINLINE class ACreep* GetAttackingCreep() const { return attackingCreep; }
	FORCEINLINE class ABase* GetEnemyBase() const { return enemyBase; }
	FORCEINLINE TArray<class ATowerBase*> GetTeamTowers() const { return teamTowers; }
	
	void AddToCapturedCamps(class ACreepCamp* camp);
	void RemoveFromCapturedCamps(class ACreepCamp* camp);
	void UpdateHeroStats();
	void SetIsCapturing(bool status, class ACreepCamp* camp);
protected:
	//AI HERO STUFFS
	class ACreepCamp* campBeingCaptured;

	UPROPERTY(EditDefaultsOnly, Category = Respawn)
		TSubclassOf<class ARespawnOverTime> respawnClass;

	UPROPERTY(EditDefaultsOnly, Category = EnemyBase)
		TSubclassOf<class ABase> enemyBaseClass;

	class ABase* enemyBase;

	class ARespawnOverTime* respawnEffect;

	UPROPERTY(EditDefaultsOnly, Category = Respawn)
		float respawnTime;
	
	class HeroStats* heroStats;

	TArray<class ACreepCamp*> capturedCamps;

	class AHeroAIController* heroAI;
	
	bool isCapturing = false;

	bool bCreepIsAttacking = false;
	bool bHeroIsAttacking = false;

	class ACreep* attackingCreep = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = TowerClass)
		TSubclassOf<class ATowerBase> towerClass;

	TArray<class ATowerBase*> teamTowers;


protected:
	//function for Trigger Events
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//function for Trigger Exit Events
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = CollisionComponents)
		USphereComponent* sphereTrigger;

	void LinkToCreepCamps();


protected:
	 FName team;
	 UPROPERTY(EditDefaultsOnly)
		 TSubclassOf<ACreepCamp> creepCampClass;
public:
	FORCEINLINE FName GetTeam() const { return team; }
	virtual void RecruitCreep();

//AI Stuff
protected:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int32 maxArmySize;

public:
	//AI HERO FUNCTIONS
	// USE ONLY WITH AI HERO TASKS!!
	bool CheckForNearbyEnemyCreeps();
	bool CheckForNearbyEnemyHero();
	bool CheckForNearbyOnwedCreepCamps();
 	bool CheckForNearbyInteractions();
	ACreep* GetClosestEnemyCreep();
	bool CheckForNearbyCreepsInArmy();
	bool CheckForNearbyEnemyTowers();
	bool FarEnemyHeroCheck();
	void AIRecruited();
	bool bJustRecruited = false;
	FORCEINLINE TArray<class ACreep*> GetNearbyEnemyCreeps() const { return nearbyEnemyCreeps; }
	FORCEINLINE AHeroBase* GetNearbyEnemyHero() const { return nearbyEnemyHero; }
	FORCEINLINE TArray<class ACreepCamp*> GetNearbyOwnedCreepCamps() const { return nearbyOwnedCreepCamps; }
	FORCEINLINE class ACreepCamp* GetNearbyEnemyCamp() const { return nearbyEnemyCamp; }
	FORCEINLINE class ATowerBase* GetNearbyEnemyTower() const { return nearbyEnemyTower; }
	FORCEINLINE bool HasJustRecruited() const { return bJustRecruited; }


private:
	//AIHERO
	TArray<class ACreep*> nearbyEnemyCreeps;
	AHeroBase* nearbyEnemyHero;
	TArray<class ACreepCamp*> nearbyOwnedCreepCamps;
	class ACreepCamp* nearbyEnemyCamp;
	class ATowerBase* nearbyEnemyTower;
	TArray<class ACreep*> nearbyCreepsInArmy;
	UFUNCTION()
		void TriggerRecruitStatusChange();

	FTimerHandle recruitTimerHandle;

public:
	TArray<ACreep*> AHeroBase::GetCreepArmyArray();
	void UpdateCreepArmy();
	ACreepCamp* visitingCamp;

	bool CheckIfTowerIsBeingAttacked();

private:
	int32 currentArmySize;
	TArray<class ACreep*> CreepArmy;

public:
	FORCEINLINE TArray<ACreep*> GetCreepArmyArray() const { return CreepArmy;  }
	void RemoveCreepFromArmy(class ACreep* creep);

	void CreepArmyChangeTeam(bool Attack = true);

protected:
	UPROPERTY(EditDefaultsOnly)
	class UCreepFormation* creepFormationComp;
public:
	FVector GetSlotPosition(int SlotNumber);


//UI STUFF
protected:
	UPROPERTY(EditDefaultsOnly)
	UDecalComponent* compassDecalComponent;
	UMaterialInstanceDynamic* compassDecalMaterialDynamic;
public:
	FORCEINLINE UDecalComponent* GetCompassDecal() const { return compassDecalComponent; }
	void ShowCompassDecal();
	void HideCompassDecal();
protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UGameTimerWidget> gameTimerWidgetClass;

	class UGameTimerWidget* gameTimerWidget;
protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UPlayerHealthBarWidget> playerHealthBarWidgetClass;

	class UPlayerHealthBarWidget* playerHealthBarWidget;

protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCampProgressWidget> campProgressWidgetClass;

	class UCampProgressWidget* campProgressWidget;



//Ability Array
protected:
	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	TArray<TSubclassOf<AAbilityBase>> AbilitiesClass;

	TArray<AAbilityBase*> Abilities;

	const int8 NUMBEROFABILITIES = 5;

public:
	FORCEINLINE AAbilityBase* GetAbility(int i) const { return (Abilities.Num() >= i ? Abilities[i] : nullptr); }
	bool SacrificeCreep();

	UPROPERTY(BlueprintReadWrite, Category = HeroCombat)
		bool bIsAttacking = false;

	virtual void UseAbility0();
	virtual void UseAbility1();
	virtual void UseAbility2();
	virtual void UseAbility3();
	virtual void UseAbility4();

	virtual void RangedAttack();
	UPROPERTY(EditDefaultsOnly, Category = Projectiles)
		TSubclassOf<class ABulletBase> BulletClass; 

	bool bIsCasting;
	FTimerHandle attackTimerHandle; 

public:
	//NOTE::BRENDON - make bIsAttacking and bIsCasting public bools
	UFUNCTION(BlueprintCallable, Category = HeroBlueprintFunctions)
	FORCEINLINE bool GetBIsAttacking() const { return bIsAttacking; }

	UFUNCTION(BlueprintCallable, Category = HeroBlueprintFunctions)
	FORCEINLINE bool GetBIsCasting() const { return bIsCasting; }

protected:
	void StopAttacking();

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<class UFloatingDamageWidget> FloatingDamageWidgetClass;

protected:
	UPROPERTY(EditAnywhere, Category = Cameras)
	class UCineCameraComponent* cyberWinCineCamera; 


	//Stuff for Melee Attack
protected:
	UPROPERTY(BlueprintReadWrite, Category = HeroCombatVariables)
		bool bIsMeleeAttacking = false;
	UPROPERTY(BlueprintReadWrite, Category = HeroCombatVariables)
		int MeleeCombo = 0; 
	UPROPERTY(EditDefaultsOnly, Category = MeleeAnimations)
		TArray<UAnimMontage*> MeleeAnimSequenceArray;
	FTimerHandle stopMovementTimerHandle;
	float DefaultMaxWalkSpeed;
	float DefaultTurnRate;
	UPROPERTY(EditDefaultsOnly, Category = MeleeAttributes)
		float MeleeAttackSlowSpeed;
	void RestoreTurnRate();
	void ResetMeleeAttackCombo();

//Melee Attacking 
public:
	void RestoreWalkSpeed();
	void RestoreFriction();
	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		virtual void ChainCombo();
	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		void Launch();
	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		void ComboReset();

	virtual void MeleeAttack();

protected:
	float launchForce;
	bool bChainAttack = false;
	int combo = 0;
	FTimerHandle restoreFrictionTimerHandle;
	FTimerHandle restoreMovementTimerHandle;

//AUDIO
protected:
	UPROPERTY(EditDefaultsOnly, Category = Audio)
		USoundWave* Announcer_EnemyHeroDestroyed;
	UPROPERTY(EditDefaultsOnly, Category = Audio)
		USoundWave* Announcer_PlayerHeroDestroyed;
	UPROPERTY(EditDefaultsOnly, Category = Audio)
		USoundWave* Announcer_LevelUp;

public:
	void TurnWithInput(bool b) { GetCharacterMovement()->bOrientRotationToMovement = b; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = ActorComponents)
		USceneComponent* CreepAttachmentComponent;
public:
	USceneComponent* GetCreepAttachment() const { return CreepAttachmentComponent; }

//Particle System for level up
protected:
	UPROPERTY(EditDefaultsOnly, Category = ParticleSystem)
	UParticleSystemComponent* LevelUpParticleSystem;

	virtual void OnDeath() { check(0 && "You must override this function!") }
public:
	virtual void OnRespawn() { check(0 && "You must override this function!") }



//protected:
////Creep Command Functions
//	virtual AActor* CreepCommand_Attack_CheckTarget(FVector Direction);
//	virtual void CreepCommand_AttackTarget();
//	TArray<FOverlapResult> creepCommand_TargetResults;
//	void UnHighlightCreepArmyTarget(AActor* enemy);
//	void HighlightCreepArmyTarget(AActor* enemy, TArray<FOverlapResult> enemies);
//	int8 commandAttackCount;

};
