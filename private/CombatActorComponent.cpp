// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatActorComponent.h"
#include "Weapon.h"
#include "Net/UnrealNetwork.h" 
#include "SRCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"  // GetMesh() 사용을 위해
#include "InputBufferActorComponent.h"


// Sets default values for this component's properties
UCombatActorComponent::UCombatActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 네트워크 복제 활성화
	SetIsReplicatedByDefault(true);
	OwnerCharacter = nullptr;
	Weapon = nullptr;
	AnimInstance = nullptr;
	WeaponClass = nullptr;
	// ...
}





void UCombatActorComponent::OnWeaponReady(TSubclassOf<AWeapon> NewWeapon)
{
	// 중복 호출 방지
	static bool bAlreadyInitialized = false;
	if (bAlreadyInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnWeaponReady: Already initialized, skipping"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("OnWeaponReady called with weapon: %s"),
		NewWeapon ? *NewWeapon->GetName() : TEXT("nullptr"));

	SetInitailizeCombatComponent();
	bAlreadyInitialized = true;
}

void UCombatActorComponent::SetInitailizeCombatComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("SetInitailizeCombatComponent called"));

	// 1단계: OwnerCharacter 초기화 및 검증
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ASRCharacter>(GetOwner());
	}

	if (!IsValid(OwnerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("CombatActorComponent: OwnerCharacter is null"));
		return;
	}

	// 2단계: Mesh 검증
	USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
	if (!IsValid(OwnerMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("CombatActorComponent: OwnerMesh is null"));
		return;
	}

	// 3단계: WeaponClass 초기화
	if (!WeaponClass)
	{
		WeaponClass = OwnerCharacter->GetWeaponClass();
		if (!WeaponClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("CombatActorComponent: WeaponClass is null"));
		}
	}

	// 4단계: Weapon 초기화 (가장 중요!)
	if (!IsValid(Weapon))
	{
		Weapon = OwnerCharacter->GetEquippedWeapon();
		if (!IsValid(Weapon))
		{
			UE_LOG(LogTemp, Error, TEXT("CombatActorComponent: Weapon is null - CRITICAL"));
			return; // Weapon이 없으면 더 이상 진행하지 않음
		}
	}

	// 5단계: AnimInstance 초기화
	if (!IsValid(AnimInstance))
	{
		AnimInstance = OwnerMesh->GetAnimInstance();
		if (!IsValid(AnimInstance))
		{
			UE_LOG(LogTemp, Warning, TEXT("CombatActorComponent: AnimInstance is null"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("SetInitailizeCombatComponent completed successfully"));
}

// Called when the game starts
void UCombatActorComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CombatActorComponent: BeginPlay Start"));

	// BeginPlay에서는 OwnerCharacter만 설정하고, 나머지는 나중에 초기화
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ASRCharacter>(GetOwner());
		if (!IsValid(OwnerCharacter))
		{
			UE_LOG(LogTemp, Error, TEXT("CombatActorComponent: Failed to get OwnerCharacter"));
			return;
		}
	}

	// WeaponReady 델리게이트에 바인딩
	if (OwnerCharacter)
	{
		OwnerCharacter->OnWeaponReady.AddDynamic(this, &UCombatActorComponent::OnWeaponReady);
		UE_LOG(LogTemp, Warning, TEXT("CombatActorComponent: Bound to OnWeaponReady delegate"));
	}

	UE_LOG(LogTemp, Warning, TEXT("CombatActorComponent: BeginPlay Finished"));

	// InputBuffer 컴포넌트 찾기
	if (IsValid(OwnerCharacter))
	{
		InputBuffer = OwnerCharacter->FindComponentByClass<UInputBufferActorComponent>();
		if (!InputBuffer)
		{
			UE_LOG(LogTemp, Warning, TEXT("InputBuffer component not found"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InputBuffer component found successfully"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CombatActorComponent: BeginPlay Finished"));
}

void UCombatActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 예시: 복제할 변수 모두 여기에 추가
	DOREPLIFETIME(UCombatActorComponent, bIsAttacking);
	DOREPLIFETIME(UCombatActorComponent, bIsGuarding);
	DOREPLIFETIME(UCombatActorComponent, bIsParrying);
	DOREPLIFETIME(UCombatActorComponent, bSaveAttack);     // 추가
	DOREPLIFETIME(UCombatActorComponent, ComboIndex);     // 추가
	//DOREPLIFETIME(ASRCharacter, bIsStunned);
	// 여기에 Replicated 변수 추가
}


// Called every frame
void UCombatActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCombatActorComponent::CanCancelCurrentAction(FName NewAction)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeSinceLastAction = CurrentTime - LastActionStartTime;

	// 공격 중에서 가드/패리로 캔슬 가능
	if (bIsAttacking && TimeSinceLastAction <= AttackCancelWindow)
	{
		if ((NewAction == TEXT("Guard") && bAllowCancelIntoGuard) ||
			(NewAction == TEXT("Parry") && bAllowCancelIntoParry))
		{
			return true;
		}
	}

	// 가드 중에서 공격/패리로 즉시 전환 가능
	if (bIsGuarding)
	{
		if (NewAction == TEXT("Attack") || NewAction == TEXT("Parry"))
		{
			return true;
		}
	}

	// 패리 중에서 공격/가드로 즉시 전환 가능
	if (bIsParrying)
	{
		if (NewAction == TEXT("Attack") || NewAction == TEXT("Guard"))
		{
			return true;
		}
	}

	return false;
}

void UCombatActorComponent::ProcessBufferedInputs()
{
	if (!InputBuffer) return;

	// 콤보 공격 중에는 InputBuffer 처리 안함 (기존 시스템 우선)
	if (bIsAttacking && bSaveAttack)
	{
		UE_LOG(LogTemp, Log, TEXT("Combo in progress - skipping buffer processing"));
		return;
	}

	FName HighestPriorityInput = InputBuffer->GetHighestPriorityInput();

	if (HighestPriorityInput != NAME_None)
	{
		if (CanCancelCurrentAction(HighestPriorityInput))
		{
			UE_LOG(LogTemp, Warning, TEXT("Processing buffered input: %s"),
				*HighestPriorityInput.ToString());

			if (HighestPriorityInput == TEXT("Attack"))
			{
				InputBuffer->ConsumeBufferedInput(TEXT("Attack"));
				ForceExecuteAttack();
			}
			else if (HighestPriorityInput == TEXT("Guard"))
			{
				InputBuffer->ConsumeBufferedInput(TEXT("Guard"));
				ForceExecuteGuard();
			}
			else if (HighestPriorityInput == TEXT("Parry"))
			{
				InputBuffer->ConsumeBufferedInput(TEXT("Parry"));
				ForceExecuteParry();
			}
		}
	}
}

void UCombatActorComponent::ForceExecuteAttack()
{
	// 현재 애니메이션 즉시 중단
	TryCancelGuard();
	TryCancelParry();

	if (bIsAttacking)
	{
		// 공격 중이면 몽타주 중단 후 새로운 공격
		TryCancelAttack();
	}

	bIsAttacking = true;
	ComboIndex = 1;
	bSaveAttack = false;
	LastActionStartTime = GetWorld()->GetTimeSeconds();

	Multicast_PlayAttackMontage(ComboIndex);
	UE_LOG(LogTemp, Warning, TEXT("Force executed attack"));
}

void UCombatActorComponent::ForceExecuteGuard()
{
	TryCancelAttack();
	TryCancelParry();

	bIsGuarding = true;
	LastActionStartTime = GetWorld()->GetTimeSeconds();

	Multicast_PlayGuardMontage();
	UE_LOG(LogTemp, Warning, TEXT("Force executed guard"));
}

void UCombatActorComponent::ForceExecuteParry()
{
	TryCancelAttack();
	TryCancelGuard();

	bIsGuarding = true;
	bIsParrying = true;
	LastActionStartTime = GetWorld()->GetTimeSeconds();

	Multicast_PlayParryMontage();
	UE_LOG(LogTemp, Warning, TEXT("Force executed parry"));
}

void UCombatActorComponent::TryCancelAttack()
{
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(CurrentAnimInstance) && IsValid(Weapon) && IsValid(Weapon->AttackMontage))
	{
		if (CurrentAnimInstance->Montage_IsActive(Weapon->AttackMontage))
		{
			// 몽타주를 멈추는 것과 동시에, 콤보 상태 변수도 여기서 직접 리셋해버립니다.
			// 델리게이트가 호출되기를 기다리지 않고 능동적으로 처리하는 것입니다.
			OwnerCharacter->SetCharacterState(ECharacterState::Idle);
			CurrentAnimInstance->Montage_Stop(0.1f, Weapon->AttackMontage);
			ResetCombo(); // ResetCombo()를 직접 호출
			//OwnerCharacter->OnCombatAnimationEnd();
		}
	}
}

void UCombatActorComponent::TryCancelGuard()
{
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(CurrentAnimInstance) && IsValid(Weapon) && IsValid(Weapon->GuardMontage))
	{
		if (CurrentAnimInstance->Montage_IsActive(Weapon->GuardMontage))
		{
			// 몽타주를 멈추는 것과 동시에, 콤보 상태 변수도 여기서 직접 리셋해버립니다.
			// 델리게이트가 호출되기를 기다리지 않고 능동적으로 처리하는 것입니다.
			OwnerCharacter->SetCharacterState(ECharacterState::Idle);
			CurrentAnimInstance->Montage_Stop(0.1f, Weapon->GuardMontage);
			bIsGuarding = false; // bIsGuard를 직접 호출
			//OwnerCharacter->OnCombatAnimationEnd();
		}
	}
}

void UCombatActorComponent::TryCancelParry()
{
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(CurrentAnimInstance) && IsValid(Weapon) && IsValid(Weapon->ParryMontage))
	{
		if (CurrentAnimInstance->Montage_IsActive(Weapon->ParryMontage))
		{
			// 몽타주를 멈추는 것과 동시에, 콤보 상태 변수도 여기서 직접 리셋해버립니다.
			// 델리게이트가 호출되기를 기다리지 않고 능동적으로 처리하는 것입니다.
			OwnerCharacter->SetCharacterState(ECharacterState::Idle);
			CurrentAnimInstance->Montage_Stop(0.1f, Weapon->ParryMontage);
			bIsParrying = false; // ResetCombo()를 직접 호출
			bIsGuarding = false;
			//OwnerCharacter->OnCombatAnimationEnd();
		}
	}
}

float UCombatActorComponent::GetOwnerAttackSpeedMultiplier() const
{
	if (IsValid(OwnerCharacter))
	{
		return OwnerCharacter->GetAttackSpeedMultiplier();
	}
	return 1.0f;
}

void UCombatActorComponent::Attack()
{
	if (!GetOwner()->HasAuthority())
	{
		Server_Attack();
		return;
	}

	if (bIsAttacking)
	{
		if (!bSaveAttack)  // 중복 방지
		{
			bSaveAttack = true;
			UE_LOG(LogTemp, Warning, TEXT("Combo attack saved"));
		}
		return;
	}

	// 새로운 공격 시작
	bIsAttacking = true;
	ComboIndex = 0;
	bSaveAttack = false;

	UE_LOG(LogTemp, Warning, TEXT("Starting new attack"));
	Multicast_PlayAttackMontage(0);

}

void UCombatActorComponent::Guard()
{
	// 클라이언트는 아무것도 검사하지 않고 서버에 요청만 보냅니다.
	if (!GetOwner()->HasAuthority())
	{
		Server_Guard();
		return;
	}

	// 서버라면 직접 실행합니다.
	Server_Guard_Implementation();
}

void UCombatActorComponent::Parry()
{
	// 클라이언트는 아무것도 검사하지 않고 서버에 요청만 보냅니다.
	if (!GetOwner()->HasAuthority())
	{
		Server_Parry();
		return;
	}

	// 서버라면 직접 실행합니다.
	Server_Parry_Implementation();
}

void UCombatActorComponent::PlayComboAttack_Implementation()
{
	FName MontageSectionName = FName(*FString::Printf(TEXT("Attack%d"), ComboIndex));
	AnimInstance->Montage_JumpToSection(MontageSectionName, Weapon->AttackMontage);
}

void UCombatActorComponent::ResetCombo()
{
	ComboIndex = 0;
	bSaveAttack = false;
	bIsAttacking = false;
	OwnerCharacter->SetCharacterState(ECharacterState::Idle);
	OwnerCharacter->SetCanMove(true);
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(CurrentAnimInstance) && IsValid(Weapon) && IsValid(Weapon->AttackMontage))
	{
		if (CurrentAnimInstance->Montage_IsActive(Weapon->AttackMontage))
		{
			CurrentAnimInstance->Montage_Stop(0.3f, Weapon->AttackMontage);
		}
	}
}

void UCombatActorComponent::NextComboAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("NextComboAttack called - bSaveAttack: %s, ComboIndex: %d"),
		bSaveAttack ? TEXT("true") : TEXT("false"), ComboIndex);

	// 첫 번째 공격에서는 bSaveAttack이 false여도 정상
	if (ComboIndex == 0 && !bSaveAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("First attack completed - not resetting"));
		return; // 첫 번째 공격은 리셋하지 않음
	}

	if (!bSaveAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("No saved attack - resetting combo"));
		ResetCombo();
		return;
	}

	// 다음 콤보 실행
	bSaveAttack = false;
	int32 NewComboIndex = ComboIndex + 1;

	if (NewComboIndex >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max combo completed - resetting"));
		ResetCombo();
		return;
	}
	ComboIndex = NewComboIndex;
	UE_LOG(LogTemp, Warning, TEXT("Executing combo %d"), ComboIndex + 1);
	Multicast_PlayAttackMontage(NewComboIndex);
}

void UCombatActorComponent::Multicast_PlayGuardMontage_Implementation()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ASRCharacter>(GetOwner());
		if (!IsValid(OwnerCharacter)) return;
	}

	// 2. 멤버 변수 Weapon, AnimInstance 대신, 함수 내에서 직접 가져와 사용합니다.
	AWeapon* CurrentWeapon = OwnerCharacter->GetEquippedWeapon();
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	if (!IsValid(CurrentWeapon) || !IsValid(CurrentWeapon->GuardMontage) || !IsValid(CurrentAnimInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid components for montage play on %s"), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client"));
		return;
	}

	// 현재 재생 중인 몽타주 중지
	if (CurrentAnimInstance->IsAnyMontagePlaying())
	{
		CurrentAnimInstance->StopAllMontages(0.1f);
	}

	CurrentAnimInstance->Montage_Play(CurrentWeapon->GuardMontage);
}

void UCombatActorComponent::Server_Guard_Implementation()
{
	if (!IsValid(OwnerCharacter)) return;

	// 서버에서는 OwnerCharacter를 통해 유효한 Weapon을 가져올 수 있습니다.
	AWeapon* CurrentWeapon = OwnerCharacter->GetEquippedWeapon();
	if (!IsValid(CurrentWeapon) || !IsValid(CurrentWeapon->GuardMontage))
	{
		return;
	}

	TryCancelAttack();
	TryCancelParry();

	bIsGuarding = true;
	Multicast_PlayGuardMontage();

}

void UCombatActorComponent::Multicast_PlayParryMontage_Implementation()
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ASRCharacter>(GetOwner());
		if (!IsValid(OwnerCharacter)) return;
	}

	// 2. 멤버 변수 Weapon, AnimInstance 대신, 함수 내에서 직접 가져와 사용합니다.
	AWeapon* CurrentWeapon = OwnerCharacter->GetEquippedWeapon();
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	if (!IsValid(CurrentWeapon) || !IsValid(CurrentWeapon->ParryMontage) || !IsValid(CurrentAnimInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid components for montage play on %s"), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client"));
		return;
	}

	// 현재 재생 중인 몽타주 중지
	if (CurrentAnimInstance->IsAnyMontagePlaying())
	{
		CurrentAnimInstance->StopAllMontages(0.1f);
	}

	CurrentAnimInstance->Montage_Play(CurrentWeapon->ParryMontage);
}

void UCombatActorComponent::Server_Parry_Implementation()
{
	if (!IsValid(OwnerCharacter)) return;

	AWeapon* CurrentWeapon = OwnerCharacter->GetEquippedWeapon();
	if (!IsValid(CurrentWeapon) || !IsValid(CurrentWeapon->ParryMontage))
	{
		return;
	}

	TryCancelAttack();
	TryCancelGuard();

	bIsParrying = true;
	bIsGuarding = true;
	Multicast_PlayParryMontage();

}


void UCombatActorComponent::Multicast_PlayAttackMontage_Implementation(int32 idx)
{
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ASRCharacter>(GetOwner());
		if (!IsValid(OwnerCharacter)) return;
	}

	// 2. 멤버 변수 Weapon, AnimInstance 대신, 함수 내에서 직접 가져와 사용합니다.
	AWeapon* CurrentWeapon = OwnerCharacter->GetEquippedWeapon();
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	if (!IsValid(CurrentWeapon) || !IsValid(CurrentWeapon->AttackMontage) || !IsValid(CurrentAnimInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid components for montage play on %s"), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client"));
		return;
	}

	// 현재 재생 중인 몽타주 중지
	if (CurrentAnimInstance->IsAnyMontagePlaying())
	{
		CurrentAnimInstance->StopAllMontages(0.1f);
	}

	// 콤보 인덱스에 따른 섹션 이름
	FName SectionName;
	switch (idx)
	{
	case 0:
		SectionName = TEXT("Attack1");
		break;
	case 1:
		SectionName = TEXT("Attack2");
		break;
	case 2:
		SectionName = TEXT("Attack3");
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid MontageComboIndex: %d"), idx);
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Playing section: %s (ComboIndex: %d)"),
		*SectionName.ToString(), ComboIndex);

	// 몽타주 재생
	float AttackSpeedMultiplier = GetOwnerAttackSpeedMultiplier();
	float PlayLength = CurrentAnimInstance->Montage_Play(CurrentWeapon->AttackMontage, AttackSpeedMultiplier);

	if (PlayLength > 0.0f)
	{
		// 섹션으로 점프
		CurrentAnimInstance->Montage_JumpToSection(SectionName, CurrentWeapon->AttackMontage);
		UE_LOG(LogTemp, Warning, TEXT("Successfully jumped to section: %s"), *SectionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to play montage"));
	}
}

void UCombatActorComponent::Multicast_PlayStunMontage_Implementation(ASRCharacter* StunChar)
{
	//StunChar->GetMesh()->GetAnimInstance()->Montage_Play(StunMontage);

	UE_LOG(LogTemp, Warning, TEXT("Multicast_PlayStunMontage called for: %s on %s"),
		StunChar ? *StunChar->GetName() : TEXT("NULL"),
		GetWorld()->GetNetMode() == NM_DedicatedServer ? TEXT("Server") : TEXT("Client"));

	if (!IsValid(StunChar) || !IsValid(StunMontage))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid StunChar or StunMontage in Multicast"));
		return;
	}

	auto StunAnimInstance = StunChar->GetMesh()->GetAnimInstance();
	if (IsValid(StunAnimInstance))
	{
		StunAnimInstance->Montage_Play(StunMontage);
		UE_LOG(LogTemp, Warning, TEXT("Stun montage played successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StunAnimInstance is not valid"));
	}
}


void UCombatActorComponent::Server_Attack_Implementation()
{
	Attack();
	/*
	// 추가 안전성 검사
	if (!IsValid(Weapon) || !IsValid(Weapon->AttackMontage))
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon or AttackMontage is null!"));
		return;
	}

	if (!IsValid(OwnerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is null in Server_Attack!"));
		return;
	}

	// 서버에서만 실행되는 공격 판정 로직
	TryCancelGuard();
	TryCancelParry();
	if (bIsAttacking)
	{
		UE_LOG(LogTemp, Log, TEXT("Attack_bIsAttacking is true"));
		bSaveAttack = true;
		return;
	}

	if (IsValid(Weapon->AttackMontage))
	{
		bIsAttacking = true;
		ComboIndex = 1;
		bSaveAttack = false;


		//FOnMontageEnded MontageEndedDelegate;
		//MontageEndedDelegate.BindUObject(this, &ASRCharacter::OnAttackMontageEnded);
		//AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Weapon->AttackMontage);
		UE_LOG(LogTemp, Log, TEXT("Attack_PlayMontageCalled"));
		Multicast_PlayAttackMontage();
	}
	*/
}


void UCombatActorComponent::Server_Stun_Implementation(ASRCharacter* StunChar)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_Stun called for: %s"),
		StunChar ? *StunChar->GetName() : TEXT("NULL"));

	if (!IsValid(StunChar))
	{
		UE_LOG(LogTemp, Error, TEXT("StunChar is not valid!"));
		return;
	}

	if (!IsValid(StunMontage))
	{
		UE_LOG(LogTemp, Error, TEXT("StunMontage is not valid!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Calling Multicast_PlayStunMontage"));
	Multicast_PlayStunMontage(StunChar);
}
