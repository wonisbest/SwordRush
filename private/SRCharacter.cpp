// Fill out your copyright notice in the Description page of Project Settings.


#include "SRCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyGameInstance.h"
#include "Weapon.h"
#include "Skill.h"
#include "SKillCoolDownManager.h"
#include "PlayerStats.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h" 
#include "SKillCoolDownManager.h"
#include "CombatActorComponent.h"
#include "AppearanceActorComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SkillList.h"
#include "InputBufferActorComponent.h"
#include "HitEffectActorComponent.h"
#include "DamageFlashComponent.h"
#include "SRPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "SkillActorComponent.h"
// Sets default values
DEFINE_LOG_CATEGORY(LogTemplateCharacter);



ASRCharacter::ASRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	// ��Ʈ��ũ ���� ����
	bReplicates = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	bReplicates = true;
	SetReplicateMovement(true);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// ������Ʈ ���� �� ���� ����
	CombatComponent = CreateDefaultSubobject<UCombatActorComponent>(TEXT("CombatComponent"));
	if (CombatComponent)
	{
		CombatComponent->SetIsReplicated(true); // �߿�!
	}

	AppearanceComponent = CreateDefaultSubobject<UAppearanceActorComponent>(TEXT("AppearanceComponent"));
	if (AppearanceComponent)
	{
		AppearanceComponent->SetIsReplicated(true);
	}

	SkillComponent = CreateDefaultSubobject<USkillActorComponent>(TEXT("SkillComponent"));
	if (SkillComponent)
	{
		SkillComponent->SetIsReplicated(true);
	}
	
	if (!CombatComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create CombatComponent!"));
	}

	if (!AppearanceComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create AppearanceComponent!"));
	}
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	HatMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HatMesh"));
	HatMesh->SetupAttachment(GetMesh());
	HatMesh->SetLeaderPoseComponent(GetMesh());

	EyeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EyeMesh"));
	EyeMesh->SetupAttachment(GetMesh());
	EyeMesh->SetLeaderPoseComponent(GetMesh());

	FaceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceMesh"));
	FaceMesh->SetupAttachment(GetMesh());
	FaceMesh->SetLeaderPoseComponent(GetMesh());

	NoseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NoseMesh"));
	NoseMesh->SetupAttachment(GetMesh());
	NoseMesh->SetLeaderPoseComponent(GetMesh());

	TopMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(GetMesh());
	TopMesh->SetLeaderPoseComponent(GetMesh());

	BottomMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BottomMesh"));
	BottomMesh->SetupAttachment(GetMesh());
	BottomMesh->SetLeaderPoseComponent(GetMesh());

	GlovesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlovesMesh"));
	GlovesMesh->SetupAttachment(GetMesh());
	GlovesMesh->SetLeaderPoseComponent(GetMesh());

	ShoesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoesMesh"));
	ShoesMesh->SetupAttachment(GetMesh());
	ShoesMesh->SetLeaderPoseComponent(GetMesh());

	InputBufferComponent = CreateDefaultSubobject<UInputBufferActorComponent>(TEXT("InputBufferComponent"));
	if (InputBufferComponent)
	{
		InputBufferComponent->SetIsReplicated(false); // UI �����̹Ƿ� ���� ���ʿ�
	}

	HitEffectComponent = CreateDefaultSubobject<UHitEffectActorComponent>(TEXT("HitEffectComponent"));
	DamageFlashComponent = CreateDefaultSubobject<UDamageFlashComponent>(TEXT("DamageFlashComponent"));
}


// Called when the game starts or when spawned
void ASRCharacter::BeginPlay()
{
	Super::BeginPlay();

	//�Է� ����
	DisableInput(Cast<APlayerController>(GetController()));
	UE_LOG(LogTemp, Warning, TEXT("Input disabled during initialization"));

	// CombatComponent�� null�̸� ����� �õ�
	if (!IsValid(CombatComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("CombatComponent is null! Attempting to recreate..."));
		CombatComponent = NewObject<UCombatActorComponent>(this, TEXT("CombatComponent"));
		if (CombatComponent)
		{
			CombatComponent->RegisterComponent();
			UE_LOG(LogTemp, Warning, TEXT("CombatComponent recreated successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to recreate CombatComponent"));
			return; // �� �̻� �������� ����
		}
	}
	// WeaponIdx�� �迭 ����
	UE_LOG(LogTemp, Warning, TEXT("WeaponIdx: %d"), WeaponIdx);
	UE_LOG(LogTemp, Warning, TEXT("BPArray_Weapons size: %d"), BPArray_Weapons.Num());

	if (BPArray_Weapons.IsValidIndex(WeaponIdx))
	{
		WeaponClass = BPArray_Weapons[WeaponIdx];
		UE_LOG(LogTemp, Warning, TEXT("WeaponClass set to: %s"),
			WeaponClass ? *WeaponClass->GetName() : TEXT("nullptr"));
	}

	// ���������� �ʱ�ȭ (�˻� ��� [4], [5] �ذ�å)
	//if (HasAuthority())
	//{

	//}
	FTimerHandle DelayedInitTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayedInitTimer, this, &ASRCharacter::DelayedInitialization, 0.5f, false);
	if (!PlayerStat)
	{
		PlayerStat = NewObject<UPlayerStats>(this, UPlayerStats::StaticClass());
	}

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->LoadPlayerData();

		FCharacterCustomization LoadedCustomization;
		LoadedCustomization.HatIndex = GI->CurrentPartIndex.HatIndex;
		LoadedCustomization.EyeIndex = GI->CurrentPartIndex.EyeIndex;
		LoadedCustomization.FaceIndex = GI->CurrentPartIndex.FaceIndex;
		LoadedCustomization.NoseIndex = GI->CurrentPartIndex.NoseIndex;
		LoadedCustomization.TopIndex = GI->CurrentPartIndex.TopIndex;
		LoadedCustomization.BottomIndex = GI->CurrentPartIndex.BottomIndex;
		LoadedCustomization.GlovesIndex = GI->CurrentPartIndex.GlovesIndex;
		LoadedCustomization.ShoesIndex = GI->CurrentPartIndex.ShoesIndex;


		// �˻� ���[11][12]�� ���� ���� ����
		FTimerHandle CustomizationTimer;
		GetWorld()->GetTimerManager().SetTimer(CustomizationTimer, [this, LoadedCustomization]()
			{
				if (HasAuthority())
				{
					// �������� ���� ����
					if (IsValid(AppearanceComponent))
					{
						CharacterCustomization = LoadedCustomization;
						OnRep_CharacterCustomization();
					}

				}
				else
				{
					if (IsValid(AppearanceComponent))
					// Ŭ���̾�Ʈ���� ������ ����
					Server_SetCharacterCustomization(LoadedCustomization);
				}
			}, 2.0f, false);


		//WeaponIdx = GI->CurrentWeaponSet.WeaponIndex;
		//SkillsListIdx = GI->CurrentSkillSet.SkillIndex;

		//PlayerStat->Attack = GI->CurrentStats.AttackPoint;
		//PlayerStat->Defense = GI->CurrentStats.DeffensePoint;
		//PlayerStat->AttackSpeed = GI->CurrentStats.AttackSpeedPoint;
		//PlayerStat->MoveSpeed = GI->CurrentStats.MoveSpeedPoint;
		//PlayerStat->StatPoint = GI->CurrentStats.StatPoint;
	}

	UE_LOG(LogTemp, Warning, TEXT("WeaponIdx: %d"), WeaponIdx);
	UE_LOG(LogTemp, Warning, TEXT("BPArray_Weapons size: %d"), BPArray_Weapons.Num());

	AnimInstance = GetMesh()->GetAnimInstance();
	if (HasAuthority())
	{
		SetWeaponMesh();
	}

		// AppearanceComponent ������ �˻�
	if (IsValid(AppearanceComponent))
	{
		AppearanceComponent->SetClothMesh();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceComponent is null!"));
	}
	//SkillCoolDownManager = NewObject<USKillCoolDownManager>(this);
	//SetSkillSet();
	UE_LOG(LogTemp, Warning, TEXT("ASRCharacter::BeginPlay() completed"));

	// �ʱ� �̵��ӵ� ����
	if (IsValid(PlayerStat))
	{
		UpdateMovementSpeed();
		UpdateAttackSpeed();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerStat is null - creating new one"));
		PlayerStat = CreateDefaultSubobject<UPlayerStats>(TEXT("PlayerStatsForced"));

		if (IsValid(PlayerStat))
		{
			PlayerStat->Attack = 1;
			PlayerStat->Defense = 1;
			PlayerStat->MoveSpeed = 1;
			PlayerStat->AttackSpeed = 1;
			UE_LOG(LogTemp, Warning, TEXT("PlayerStat recreated with default values"));
		}
	}
	// CombatComponent�� InputBuffer ����
	if (IsValid(CombatComponent) && IsValid(InputBufferComponent))
	{
		CombatComponent->InputBuffer = InputBufferComponent;
		UE_LOG(LogTemp, Warning, TEXT("InputBuffer connected to CombatComponent"));
	}

	// ��� �õ�
	if (IsLocallyControlled())
	{
		if (IsValid(GetGameInstance()) && IsValid(GetPlayerState()))
		{
			InitializePlayerStatsFromGameInstance();
			UE_LOG(LogTemp, Warning, TEXT("Stats initialized immediately"));
		}
		else
		{
			// 0.5�� �� �� �� �� �õ�
			FTimerHandle QuickRetryTimer;
			GetWorld()->GetTimerManager().SetTimer(QuickRetryTimer, [this]()
				{
					if (IsValid(GetGameInstance()) && IsValid(GetPlayerState()))
					{
						InitializePlayerStatsFromGameInstance();
						UE_LOG(LogTemp, Warning, TEXT("Stats initialized after quick retry"));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Stats initialization failed"));
					}
				}, 0.5f, false); // 0.5�ʸ� ���
		}
	}

	//�Է� Ȱ��ȭ
	EnableInput(Cast<APlayerController>(GetController()));
	UE_LOG(LogTemp, Warning, TEXT("Input enabled initialization finished"));
}
void ASRCharacter::HandleAttack(const FInputActionValue& Value)
{
	if (bIsStunned || CurrentState == ECharacterState::Stunned)
	{
		return;
	}

	if (IsValid(CombatComponent))
	{
		SetCharacterState(ECharacterState::Attacking);
		CombatComponent->Attack();  // �̰͸� ȣ��

		// �������� ��� ���� (Attack() ���ο��� ó��)
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CombatComponent is null!"));
	}
}
void ASRCharacter::HandleGuard(const FInputActionValue& Value)
{
	if (bIsStunned || CurrentState == ECharacterState::Stunned)
	{
		return;
	}

	if (IsValid(CombatComponent))
	{
		CombatComponent->Guard();
		SetCharacterState(ECharacterState::Guarding);
		// ���� �켱������ InputBuffer�� ���� (����� �ൿ)
		if (InputBufferComponent)
		{
			InputBufferComponent->BufferInput(TEXT("Guard"), 2.0f);
		}
	}
}
void ASRCharacter::HandleParry(const FInputActionValue& Value)
{
	if (bIsStunned || CurrentState == ECharacterState::Stunned)
	{
		return;
	}

	if (IsValid(CombatComponent))
	{

		CombatComponent->Parry();
		SetCharacterState(ECharacterState::Parrying);
		// �ְ� �켱������ InputBuffer�� ���� (��Ȯ�� Ÿ�̹� �ʿ�)
		if (InputBufferComponent)
		{
			InputBufferComponent->BufferInput(TEXT("Parry"), 3.0f);
		}
	}
}
void ASRCharacter::HandleSkill()
{
	if (SkillComponent)
	{
		SkillComponent->RequestUseSkill();
	}
}
// Called to bind functionality to input
void ASRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASRCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASRCharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASRCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASRCharacter::Look);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASRCharacter::HandleAttack);
		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Started, this, &ASRCharacter::HandleGuard);
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &ASRCharacter::HandleParry);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &ASRCharacter::HandleSkill);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASRCharacter::GamePause);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASRCharacter::OnRep_Weapon()
{
	if (IsValid(Weapon))
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon"));
	}
}
void ASRCharacter::OnRep_CharacterCustomization()
{
	if (!AppearanceComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceComponent is nullptr!"));
		return;
	}
	if (!IsValid(AppearanceComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceComponent is not valid!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("OnRep_CharacterCustomization called"));
	ApplyCustomizationToMesh(CharacterCustomization);
}

void ASRCharacter::Server_SetCharacterCustomization_Implementation(const FCharacterCustomization& NewCustomization)
{
	if (!AppearanceComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceComponent is nullptr!"));
		return;
	}
	if (!IsValid(AppearanceComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceComponent is not valid!"));
		return;
	}
	CharacterCustomization = NewCustomization;
	ApplyCustomizationToMesh(CharacterCustomization);
	// ��� Ŭ���̾�Ʈ�� ����
	Multicast_ApplyCustomization(CharacterCustomization);
}

void ASRCharacter::Multicast_ApplyCustomization_Implementation(const FCharacterCustomization& NewCustomization)
{
	if (!AppearanceComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceComponent is nullptr!"));
		return;
	}
	if (!IsValid(AppearanceComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceComponent is not valid!"));
		return;
	}

	ApplyCustomizationToMesh(NewCustomization);
}

void ASRCharacter::ApplyCustomizationToMesh(const FCharacterCustomization& Customization)
{
	if (!AppearanceComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceComponent is nullptr!"));
		return;
	}
	if (!IsValid(AppearanceComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceComponent is not valid!"));
		return;
	}

	// CRASH FIX: AppearanceComponent�� ��ȿ���� ���� �� �����ϴ� ���� ���� �ֿ켱 ��� �ڵ�
	if (!IsValid(AppearanceComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("CRITICAL: AppearanceComponent is NOT VALID in ApplyCustomizationToMesh for character %s. Aborting customization."), *GetName());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Applying customization to %s - Hat: %d, Eye: %d"), *GetName(), Customization.HatIndex, Customization.EyeIndex);

	AppearanceComponent->HatIdx = Customization.HatIndex;
	AppearanceComponent->EyeIdx = Customization.EyeIndex;
	AppearanceComponent->FaceIdx = Customization.FaceIndex;
	AppearanceComponent->NoseIdx = Customization.NoseIndex;
	AppearanceComponent->TopIdx = Customization.TopIndex;
	AppearanceComponent->BottomIdx = Customization.BottomIndex;
	AppearanceComponent->GlovesIdx = Customization.GlovesIndex;
	AppearanceComponent->ShoesIdx = Customization.ShoesIndex;

	// �޽� ������Ʈ �Լ� ȣ��
	AppearanceComponent->SetClothMesh();
}

void ASRCharacter::UpdateMovementSpeed()
{
	if (!IsValid(PlayerStat) || !IsValid(GetCharacterMovement()))
	{
		return;
	}

	// �⺻ �̵��ӵ� (600�̶�� ����)
	float BaseWalkSpeed = 600.0f;

	// ���� 1�� 1% ���� ���
	float SpeedMultiplier = 1.0f + (PlayerStat->MoveSpeed * 0.01f);
	float NewWalkSpeed = BaseWalkSpeed * SpeedMultiplier;

	// �˻� ���[1]�� ���� Max Walk Speed ����
	GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;

	//UE_LOG(LogTemp, Warning, TEXT("Movement speed updated - MoveSpeedStat: %d, Multiplier: %.2f, New Speed: %.2f"),PlayerStat->MoveSpeed, SpeedMultiplier, NewWalkSpeed);
}

float ASRCharacter::GetAttackSpeedMultiplier() const
{
	if (!IsValid(PlayerStat))
	{
		return 1.0f;
	}

	// ���� 1�� 2% ����
	return 1.0f + (PlayerStat->AttackSpeed * 0.02f);
}

void ASRCharacter::UpdateAttackSpeed()
{
	// ���� ��� ���� ��Ÿ���� �ӵ� ���� (�˻� ���[7] ����)
	if (IsValid(AnimInstance))
	{
		if (UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage())
		{
			float SpeedMultiplier = GetAttackSpeedMultiplier();
			AnimInstance->Montage_SetPlayRate(CurrentMontage, SpeedMultiplier);
		}
	}
}

void ASRCharacter::OnPlayerStatChanged()
{
	UpdateMovementSpeed();
	//UpdateAttackSpeed();
}

void ASRCharacter::SetWeaponMesh()
{
	if (!HasAuthority()) return;

	UE_LOG(LogTemp, Warning, TEXT("SetWeaponMesh() called"));
	// �߰� ������ �˻�
	if (!IsValid(CombatComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("CombatComponent is not valid"));
		return;
	}

	if (Weapon)
	{
		Weapon->Destroy();
		Weapon = nullptr;
	}
	if (!BPArray_Weapons.IsValidIndex(WeaponIdx))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid WeaponIdx: %d"), WeaponIdx);
		return;
	}
	WeaponClass = BPArray_Weapons[WeaponIdx];

	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (Weapon)
		{
			// ĳ������ Mesh�� ���� �̸����� ����
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon"));
			if (IsValid(Weapon))
			{
				OnWeaponReady.Broadcast(WeaponClass);
				// ������ ���� ĸó ���
				TWeakObjectPtr<ASRCharacter> WeakThis = this;
				TWeakObjectPtr<UCombatActorComponent> WeakCombat = WeakThis->CombatComponent;
				TWeakObjectPtr<AWeapon> WeakWeapon = Weapon;
				// �߰� �������� ���� ���� �ʱ�ȭ ���
				FTimerHandle WeaponInitTimer;
				GetWorld()->GetTimerManager().SetTimer(WeaponInitTimer, [WeakThis, WeakCombat, WeakWeapon]()
					{
						if (WeakThis.IsValid() && WeakCombat.IsValid() && WeakWeapon.IsValid())
						{
							WeakCombat->SetInitailizeCombatComponent();
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("WeakPtr validation failed in lambda"));
						}
					}, 0.1f, false);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Weapon Is Not Valid after spawn"));
			}
			OnWeaponReady.Broadcast(WeaponClass);
			CombatComponent->SetInitailizeCombatComponent();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon!"));
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponClass is nullptr!"));
	}

}
void ASRCharacter::RecieveHitFromWeapon(AWeapon* HitWeapon, const FHitResult& Hit)
{
	if (CombatComponent->bIsParrying)
	{
		return;
	}

	if (CombatComponent->bIsGuarding)
	{
		return;
	}
}


void ASRCharacter::Move(const FInputActionValue& Value)
{
	// �������� ���ѵ� ���¶�� �Է� ����
	if (!bCanMove || bIsStunned ||
		CurrentState == ECharacterState::Attacking ||
		CurrentState == ECharacterState::Guarding ||
		CurrentState == ECharacterState::Parrying ||
		CurrentState == ECharacterState::Stunned ||
		CurrentState == ECharacterState::Skill)
	{
		return;
	}
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		// ���� ������Ʈ �߰�
		if (MovementVector.Size() > 0.0f && CurrentState == ECharacterState::Idle)
		{
			SetCharacterState(ECharacterState::Moving);
		}
		else if (MovementVector.Size() == 0.0f && CurrentState == ECharacterState::Moving)
		{
			SetCharacterState(ECharacterState::Idle);
		}
	}
}

void ASRCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASRCharacter::Jump()
{
	// �������� ���ѵ� ���¶�� �Է� ����
	if (!bCanMove || bIsStunned ||
		CurrentState == ECharacterState::Attacking ||
		CurrentState == ECharacterState::Guarding ||
		CurrentState == ECharacterState::Parrying ||
		CurrentState == ECharacterState::Stunned ||
		CurrentState == ECharacterState::Skill)
	{
		return;
	}

	Super::Jump();

}

void ASRCharacter::StopJumping()
{
	if (!bCanMove || bIsStunned ||
		CurrentState == ECharacterState::Attacking ||
		CurrentState == ECharacterState::Guarding ||
		CurrentState == ECharacterState::Parrying ||
		CurrentState == ECharacterState::Stunned ||
		CurrentState == ECharacterState::Skill)
	{
		return;
	}

	Super::StopJumping();
	// �������� ���ѵ� ���¶�� �Է� ����

}

void ASRCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;
	UE_LOG(LogTemp, Log, TEXT("Character state changed to: %d"), (int32)NewState);

	switch (NewState)
	{
	case ECharacterState::Attacking:
	case ECharacterState::Skill:
	case ECharacterState::Stunned:
	case ECharacterState::Parrying:
	case ECharacterState::Guarding:
		bCanMove = false;
		break;
	case ECharacterState::Idle:
	case ECharacterState::Moving:
		bCanMove = true;
		// Idle ���°� �Ǹ� InputBuffer Ŭ����
		if (InputBufferComponent)
		{
			InputBufferComponent->ClearAllBuffers();
		}
		break;
	}

	SetCanMove(bCanMove);

}

void ASRCharacter::SetCanMove(bool bNewCanMove)
{
	bCanMove = bNewCanMove;

	if (!bCanMove)
	{
		// ������ ���� ����
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		// �ٽ� ������ ���
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void ASRCharacter::ApplyStun(float StunDuration, ASRCharacter* StunChar)
{
	if (!HasAuthority()) return; // ���������� ����

	StunChar->bIsStunned = true;
	StunChar->CombatComponent->bIsAttacking = false;
	StunChar->CombatComponent->bIsGuarding = false;
	StunChar->CombatComponent->bIsParrying = false;
	StunChar->SetCharacterState(ECharacterState::Stunned);
	StunChar->SetCanMove(false);
	// ���� üũ ���� �ٷ� ȣ��
	if (IsValid(StunChar->CombatComponent))
	{
		StunChar->CombatComponent->Server_Stun(StunChar);
	}
	// ���� Ÿ�̸� ����
	StunChar->GetWorldTimerManager().SetTimer(StunTimerHandle, [this, StunChar](){RemoveStun(StunChar);}, StunDuration, false);

	UKismetSystemLibrary::PrintString(this,
		FString::Printf(TEXT("Stunned for %.1f seconds"), StunDuration),
		true, true, FLinearColor::Yellow, 3.0f);
}

void ASRCharacter::RemoveStun(ASRCharacter* StunChar)
{
	if (!HasAuthority()) return; // ���������� ����

	StunChar->bIsStunned = false;
	StunChar->SetCharacterState(ECharacterState::Idle);
	StunChar->SetCanMove(true);

	// Ÿ�̸� Ŭ����
	StunChar->GetWorldTimerManager().ClearTimer(StunTimerHandle);

	UKismetSystemLibrary::PrintString(this, TEXT("Stun Removed"),
		true, true, FLinearColor::Green, 2.0f);
}

void ASRCharacter::OnCombatAnimationEnd()
{
	SetCharacterState(ECharacterState::Idle);
	SetCanMove(true);

	UE_LOG(LogTemp, Log, TEXT("Attack Animation Ended"));
}

void ASRCharacter::OnSkillAnimationEnd()
{
	SetCharacterState(ECharacterState::Idle);
	SetCanMove(true);

	UE_LOG(LogTemp, Log, TEXT("Skill Animation Ended"));
}

void ASRCharacter::OnStunAnimationEnd()
{
	// ���� �ִϸ��̼� ���� ���������� ó��
	UE_LOG(LogTemp, Log, TEXT("Stun Animation Ended"));

	// ���� ���´� Ÿ�̸ӷ� �����ǹǷ� ���⼭ ���� �������� ����
	// ��� �ð��� ȿ���� �߰� ó���� ����

	// ����: ���� ����Ʈ ����
	// if (StunEffect)
	// {
	//     StunEffect->Deactivate();
	// }
}

void ASRCharacter::PlayComboAttack()
{
}

void ASRCharacter::OnComboInputWindowOpened(bool bIsOpend)
{
}

void ASRCharacter::OnComboInputWindowClosed(bool bIsClose)
{
}

void ASRCharacter::ApplyReplicatedStats(const FSRPlayerStats& Stats)
{
	if (IsValid(PlayerStat))
	{
		PlayerStat->Attack = Stats.AttackPoint;
		PlayerStat->Defense = Stats.DeffensePoint;
		PlayerStat->AttackSpeed = Stats.AttackSpeedPoint;
		PlayerStat->MoveSpeed = Stats.MoveSpeedPoint;
		PlayerStat->StatPoint = Stats.StatPoint;

		UpdateMovementSpeed();

		UE_LOG(LogTemp, Warning, TEXT("Applied replicated stats - Attack: %d, Defense: %d"),
			Stats.AttackPoint, Stats.DeffensePoint);
	}
}

void ASRCharacter::InitializePlayerStatsFromGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("InitializePlayerStatsFromGameInstance called"));

	// �˻� ���[1]�� ���� null üũ ��ȭ
	if (!IsValid(this))
	{
		UE_LOG(LogTemp, Error, TEXT("Character is invalid"));
		return;
	}

	if (!IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not locally controlled - skipping"));
		return;
	}

	// GameInstance null üũ
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (!IsValid(GI))
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is null - retrying in 1 second"));

		FTimerHandle RetryTimer;
		GetWorld()->GetTimerManager().SetTimer(RetryTimer, [this]()
			{
				InitializePlayerStatsFromGameInstance();
			}, 1.0f, false);
		return;
	}

	// PlayerState null üũ
	ASRPlayerState* PS = GetPlayerState<ASRPlayerState>();
	if (!IsValid(PS))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState is null - retrying in 1 second"));

		FTimerHandle RetryTimer;
		GetWorld()->GetTimerManager().SetTimer(RetryTimer, [this]()
			{
				InitializePlayerStatsFromGameInstance();
			}, 1.0f, false);
		return;
	}

	// �����ϰ� ������ �ε� �� ����
	if (GI && PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading player data and sending to server..."));
		GI->LoadPlayerData();
		PS->Server_SetStats(GI->CurrentStats);
		PS->Server_SetWeaponIndex(GI->CurrentWeaponSet.WeaponIndex);
		UE_LOG(LogTemp, Warning, TEXT("Stats successfully sent to server"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GI or PS is invalid even after checks!"));
	}


}

void ASRCharacter::GamePause()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("GamePause: PlayerController is null!"));
		return;
	}

	// ������ �̹� �����Ǿ� ȭ�鿡 ǥ�õǰ� �ִ��� Ȯ���մϴ�.
	if (CurrentPauseWidgetInstance)
	{
		// ������ ǥ�� ���̸� �����մϴ�.
		CurrentPauseWidgetInstance->RemoveFromParent();
		CurrentPauseWidgetInstance = nullptr; // �ν��Ͻ� ���� ����

		// ���� �Ͻ����� ����
		//PlayerController->SetPause(false);
		// ���콺 Ŀ�� ����
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly()); // ���� ���� �Է� ���� ����
		UE_LOG(LogTemp, Warning, TEXT("Pause Widget removed. Game unpaused."));
	}
	else // ������ ǥ�õǰ� ���� �ʴٸ�
	{
		// ���� Ŭ������ �����Ǿ� �ִ��� Ȯ���մϴ�.
		if (!PauseWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("GamePause: PauseWidgetClass is not set in SRCharacter Blueprint!"));
			return;
		}

		// ������ �����ϰ� ȭ�鿡 �߰��մϴ�.
		CurrentPauseWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);

		if (CurrentPauseWidgetInstance)
		{
			CurrentPauseWidgetInstance->AddToViewport();

			// ���� �Ͻ�����
			//PlayerController->SetPause(true);
			// ���콺 Ŀ�� ǥ��
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeGameAndUI()); // UI ���� �Է� ���� ���� (UI ��ȣ�ۿ� ����)
			UE_LOG(LogTemp, Warning, TEXT("Pause Widget created and added to viewport. Game paused."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GamePause: Failed to create Pause Widget!"));
		}
	}
}

void ASRCharacter::DelayedInitialization()
{
	if (!IsValid(this))
	{
		return;
	}

	bool bMeshReady = IsValid(GetMesh());
	bool bAnimInstanceReady = bMeshReady && IsValid(GetMesh()->GetAnimInstance());
	bool bCombatComponentReady = IsValid(CombatComponent);
	bool bWeaponClassReady = IsValid(WeaponClass);

	if (bMeshReady && bAnimInstanceReady && bCombatComponentReady && bWeaponClassReady)
	{
		SetWeaponMesh();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Components still not ready"));
	}
}


void ASRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ReplicatedUsing�� ����ϴ� ������ DOREPLIFETIME ���
	DOREPLIFETIME(ASRCharacter, CurrentState);

	// �Ϲ� Replicated ������
	DOREPLIFETIME(ASRCharacter, bCanMove);
	DOREPLIFETIME(ASRCharacter, bIsStunned);
	DOREPLIFETIME(ASRCharacter, Health);
	DOREPLIFETIME(ASRCharacter, CharacterCustomization);
	DOREPLIFETIME(ASRCharacter, Weapon);
}

float ASRCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage called - HasAuthority: %s, DamageAmount: %f"),
		HasAuthority() ? TEXT("true") : TEXT("false"), DamageAmount);

	// ���������� ���� ������ ó��
	if (HasAuthority())
	{
		return ProcessDamage(DamageAmount, EventInstigator, DamageCauser);
	}
	else
	{
		// Ŭ���̾�Ʈ������ �ƹ��͵� ���� ����
		UE_LOG(LogTemp, Warning, TEXT("Client ignoring damage - will be processed on server"));
		return 0.0f;
	}
}

void ASRCharacter::Server_TakeDamage_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_TakeDamage_Implementation called"));
	ProcessDamage(DamageAmount, EventInstigator, DamageCauser);
}

float ASRCharacter::ProcessDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessDamage - Health before: %f, Damage: %f"), Health, DamageAmount);
	// ���������� ������ �� ��� ����
	if (DamageAmount > 1000.0f || DamageAmount < -1000.0f || FMath::IsNaN(DamageAmount))
	{
		UE_LOG(LogTemp, Error, TEXT("Blocking abnormal damage: %.2f"), DamageAmount);
		DamageAmount = 1.0f; // ������ �⺻��
	}

	// PlayerStat null üũ
	if (!IsValid(PlayerStat))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerStat is null in ProcessDamage - using safe damage"));

		if (DamageAmount > 0.0f)
		{
			float OldHealth = Health;
			Health = FMath::Clamp(Health - 1.0f, 0.0f, 100.0f); // ������ ������
			OnRep_Health();
			return 1.0f;
		}
		return 0.0f;
	}
	// PlayerStat ��ȿ�� �˻� �� �⺻�� ����
	if (!IsValid(PlayerStat))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerStat is null - using default values"));

		// �⺻ ������ ó�� (PlayerStat ����)
		float CalculatedDamage = FMath::Clamp(DamageAmount, 1.0f, 50.0f);

		if (CalculatedDamage > 0.0f)
		{
			float OldHealth = Health;
			Health = FMath::Clamp(Health - CalculatedDamage, 0.0f, 100.0f);

			UE_LOG(LogTemp, Warning, TEXT("Health changed from %f to %f (without PlayerStat)"), OldHealth, Health);
			OnRep_Health();
		}

		return CalculatedDamage;
	}


	// ����/�и� ���� Ȯ��
	bool bDamageBlocked = false;
	ASRCharacter* AttackerCharacter = Cast<ASRCharacter>(DamageCauser);
	if (IsValid(CombatComponent))
	{
		if (CombatComponent->bIsGuarding)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attack blocked by guard"));

			if (CombatComponent->bIsParrying)
			{
				UE_LOG(LogTemp, Warning, TEXT("Perfect parry! Stunning attacker"));
				if (AttackerCharacter)
				{
					ApplyStun(1.0f, AttackerCharacter);

					// [���� ����] IsLocallyControlled�� Ȯ��
					if (AttackerCharacter->IsLocallyControlled())
					{
						AttackerCharacter->HitEffectComponent->PlayParryEffects(GetActorLocation());
					}
					else
					{
						AttackerCharacter->Client_PlayParryEffect(GetActorLocation());
					}
				}
				// �ǰ��ڴ� �и� ����Ʈ ���
				if (IsValid(HitEffectComponent))
				{
					HitEffectComponent->PlayParryEffects(Weapon->GetActorLocation());
				}
				return 0.0f;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Normal guard - reduced damage"));
				DamageAmount *= 0.5f;
				bDamageBlocked = true;
				if (AttackerCharacter)
				{
					// [���� ����] IsLocallyControlled�� Ȯ��
					if (AttackerCharacter->IsLocallyControlled())
					{
						AttackerCharacter->HitEffectComponent->PlayGuardEffects(GetActorLocation());
					}
					else
					{
						AttackerCharacter->Client_PlayGuardEffect(GetActorLocation());
					}
				}
				// �ǰ��ڴ� ���� ����Ʈ ���
				if (IsValid(HitEffectComponent))
				{
					HitEffectComponent->PlayGuardEffects(Weapon->GetActorLocation());
				}
			}

		}
	}

	// ������ ���
	float AttackerAttack = 1.0f;
	
	if (IsValid(AttackerCharacter))
	{
		if (ASRPlayerState* AttackerState = AttackerCharacter->GetPlayerState<ASRPlayerState>())
		{
			AttackerAttack = AttackerState->CurrentStats.AttackPoint;
			UE_LOG(LogTemp, Warning, TEXT("Attacker Attack: %f"), AttackerAttack);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Attacker PlayerStat is null"));
			AttackerAttack = 1.0f;
		}
	}
	float MyDefense = 1.0f;
	if (ASRPlayerState* MyState = GetPlayerState<ASRPlayerState>())
	{
		MyDefense = MyState->CurrentStats.DeffensePoint;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Defenser PlayerStat is null"));
	}
	float CalculatedDamage = AttackerAttack - (MyDefense / 2.0f);
	CalculatedDamage = FMath::Max(1.0f, CalculatedDamage);

	UE_LOG(LogTemp, Warning, TEXT("Calculated Damage: %f"), CalculatedDamage);

	// �߰� ������ġ
	CalculatedDamage = FMath::Clamp(CalculatedDamage, 0.0f, 50.0f);

	// ü�� ���� (����������, �׸��� ���� �������� ���� ����)
	if (CalculatedDamage > 0.0f)
	{
		float OldHealth = Health;
		Health = FMath::Clamp(Health - CalculatedDamage, 0.0f, 100.0f);

		UE_LOG(LogTemp, Warning, TEXT("Health changed from %f to %f"), OldHealth, Health);

		// ���������� �������� OnRep_Health ȣ�� (Ŭ���̾�Ʈ�鿡�Դ� �ڵ� ������)
		OnRep_Health();

		// �߰�: ������ ���� ������Ʈ
		ForceNetUpdate();

		// �ǰ��� ����Ʈ ���
		if (IsValid(HitEffectComponent))
		{
			HitEffectComponent->PlayHitEffects(CalculatedDamage, GetActorLocation());
		}

		// ������ �ǵ��
		if (AttackerCharacter)
		{
			// [���� ����] IsLocallyControlled�� Ȯ��
			if (AttackerCharacter->IsLocallyControlled()) // �����ڰ� ���� �÷��̾���
			{
				AttackerCharacter->HitEffectComponent->PlayHitEffects(CalculatedDamage, GetActorLocation());
			}
			else // �����ڰ� Ŭ���̾�Ʈ �÷��̾���
			{
				AttackerCharacter->Client_PlayHitEffect(CalculatedDamage, GetActorLocation());
			}
		}
	}
	if (CombatComponent->bIsGuarding)
	{
		return CalculatedDamage * 0.5f;
	}
	return CalculatedDamage;
}

AWeapon* ASRCharacter::GetEquippedWeapon() const
{
	if (IsValid(Weapon)) 
	{
		return Weapon;
	}
	UE_LOG(LogTemp, Error, TEXT("GetEquippedWeapon is Failed"));
	return nullptr;
}

TSubclassOf<AWeapon> ASRCharacter::GetWeaponClass() const
{
	if (IsValid(WeaponClass))
	{
		return WeaponClass;
	}
	UE_LOG(LogTemp, Error, TEXT("GetWeaponClass is Failed"));
	return nullptr;
}

void ASRCharacter::OnRep_Health()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Health called - Health: %f, Character: %s"),
		Health, *GetName());

	// ��������Ʈ ��ε�ĳ��Ʈ
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(Health);
		UE_LOG(LogTemp, Warning, TEXT("OnHealthChanged broadcasted for %s"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHealthChanged not bound for %s - Delegate count: %d"),
			*GetName(), OnHealthChanged.GetAllObjects().Num());
	}
}

void ASRCharacter::Client_PlayHitEffect_Implementation(float DamageAmount, FVector HitLocation)
{
	HitEffectComponent->PlayHitEffects(DamageAmount, HitLocation);
}

void ASRCharacter::Client_PlayGuardEffect_Implementation(FVector HitLocation)
{
	HitEffectComponent->PlayGuardEffects(HitLocation);
}

void ASRCharacter::Client_PlayParryEffect_Implementation(FVector HitLocation)
{
	HitEffectComponent->PlayParryEffects(HitLocation);
}
// Called every frame
void ASRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



