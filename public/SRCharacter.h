// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SRCharacter.generated.h"

class UPlayerStats;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UWeapon;
class USkill;
class USKillCoolDownManager;
class UPlayerStats;
struct FInputActionValue;
struct FSRPlayerStats;
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReady, TSubclassOf<AWeapon>, NewWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Moving      UMETA(DisplayName = "Moving"),
	Attacking   UMETA(DisplayName = "Attacking"),
	Guarding    UMETA(DisplayName = "Guarding"),
	Parrying    UMETA(DisplayName = "Parrying"),
	Stunned     UMETA(DisplayName = "Stunned"),
	Skill       UMETA(DisplayName = "Skill")
};

USTRUCT(BlueprintType)
struct FCharacterCustomization
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 HatIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 EyeIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 FaceIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 NoseIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 TopIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 BottomIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 GlovesIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 ShoesIndex = 0;
};

UCLASS(config = Game)
class SWORDRUSH_API ASRCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GuardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ParryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;
	/** Hat, Top, Bottom, etc... **/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;


	UPROPERTY(ReplicatedUsing = OnRep_Weapon)
	AWeapon* Weapon;

	UFUNCTION()
	void OnRep_Weapon();

public:
	// Sets default values for this character's properties
	ASRCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class UInputBufferActorComponent* InputBufferComponent;

	UPlayerStats* PlayerStat;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponReady OnWeaponReady;

	bool bInputQueued = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAppearanceActorComponent* AppearanceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCombatActorComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkillActorComponent* SkillComponent;

	UAnimMontage* AttackMontage;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(Server, Reliable)
	void Server_TakeDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);

	float ProcessDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);

	class AWeapon* GetEquippedWeapon() const;
	class TSubclassOf<AWeapon> GetWeaponClass() const;
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Health")
	float Health = 100.0f;

	UFUNCTION()
	void OnRep_Health();


	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	//UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	//ECharacterState CurrentState = ECharacterState::Idle;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bCanMove = true;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsStunned = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	ECharacterState CurrentState = ECharacterState::Idle;

	UPROPERTY(ReplicatedUsing = OnRep_CharacterCustomization, BlueprintReadOnly)
	FCharacterCustomization CharacterCustomization;

	UFUNCTION()
	void OnRep_CharacterCustomization();

	UFUNCTION(Server, Reliable)
	void Server_SetCharacterCustomization(const FCharacterCustomization& NewCustomization);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyCustomization(const FCharacterCustomization& NewCustomization);

	void ApplyCustomizationToMesh(const FCharacterCustomization& Customization);

	UFUNCTION(BlueprintCallable)
	void UpdateMovementSpeed();

	UFUNCTION(BlueprintCallable)
	float GetAttackSpeedMultiplier() const;

	UFUNCTION(BlueprintCallable)
	void UpdateAttackSpeed();

	UFUNCTION(BlueprintCallable)
	void OnPlayerStatChanged();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	class UHitEffectActorComponent* HitEffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	class UDamageFlashComponent* DamageFlashComponent;

	// 헤더 파일에 추가
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCharacterState(ECharacterState NewState);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCanMove(bool bNewCanMove);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HandleAttack(const FInputActionValue& Value);
	void HandleGuard(const FInputActionValue& Value);
	void HandleParry(const FInputActionValue& Value);
	void HandleSkill();
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Jump() override;
	void StopJumping() override;

	UAnimInstance* AnimInstance;



	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyStun(float StunDuration, ASRCharacter* StunChar);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveStun(ASRCharacter* StunChar);

	// 타이머 핸들
	FTimerHandle StunTimerHandle;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable, Category = "Animation")
	void OnCombatAnimationEnd();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void OnSkillAnimationEnd();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void OnStunAnimationEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AWeapon>> BPArray_Weapons;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 WeaponIdx;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeaponMesh();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RecieveHitFromWeapon(AWeapon* HitWeapon, const FHitResult& Hit);
	

	void PlayComboAttack();
	void OnComboInputWindowOpened(bool bIsOpend);
	void OnComboInputWindowClosed(bool bIsClose);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// 의상 메시 컴포넌트들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USkeletalMeshComponent* HatMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USkeletalMeshComponent* EyeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USkeletalMeshComponent* FaceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USkeletalMeshComponent* NoseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USkeletalMeshComponent* TopMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USkeletalMeshComponent* BottomMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USkeletalMeshComponent* GlovesMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USkeletalMeshComponent* ShoesMesh;


	UFUNCTION(BlueprintCallable)
	void ApplyReplicatedStats(const FSRPlayerStats& Stats);

	void InitializePlayerStatsFromGameInstance();

	void GamePause();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;
	bool bIsPause = false;

	UUserWidget* CurrentPauseWidgetInstance;

	UFUNCTION(Client, REliable)
	void Client_PlayHitEffect(float DamageAmount, FVector HitLocation);

	UFUNCTION(Client, REliable)
	void Client_PlayGuardEffect(FVector HitLocation);

	UFUNCTION(Client, REliable)
	void Client_PlayParryEffect(FVector HitLocation);

private:
	void DelayedInitialization();
};
