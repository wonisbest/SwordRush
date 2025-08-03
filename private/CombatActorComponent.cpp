// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatActorComponent.h"
#include "Weapon.h"
#include "Net/UnrealNetwork.h" 
#include "SRCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"  // GetMesh() ����� ����
#include "InputBufferActorComponent.h"


// Sets default values for this component's properties
UCombatActorComponent::UCombatActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ��Ʈ��ũ ���� Ȱ��ȭ
	SetIsReplicatedByDefault(true);
	OwnerCharacter = nullptr;
	Weapon = nullptr;
	AnimInstance = nullptr;
	WeaponClass = nullptr;
	// ...
}





void UCombatActorComponent::OnWeaponReady(TSubclassOf<AWeapon> NewWeapon)
{
	// �ߺ� ȣ�� ����
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

	// 1�ܰ�: OwnerCharacter �ʱ�ȭ �� ����
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ASRCharacter>(GetOwner());
	}

	if (!IsValid(OwnerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("CombatActorComponent: OwnerCharacter is null"));
		return;
	}

	// 2�ܰ�: Mesh ����
	USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
	if (!IsValid(OwnerMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("CombatActorComponent: OwnerMesh is null"));
		return;
	}

	// 3�ܰ�: WeaponClass �ʱ�ȭ
	if (!WeaponClass)
	{
		WeaponClass = OwnerCharacter->GetWeaponClass();
		if (!WeaponClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("CombatActorComponent: WeaponClass is null"));
		}
	}

	// 4�ܰ�: Weapon �ʱ�ȭ (���� �߿�!)
	if (!IsValid(Weapon))
	{
		Weapon = OwnerCharacter->GetEquippedWeapon();
		if (!IsValid(Weapon))
		{
			UE_LOG(LogTemp, Error, TEXT("CombatActorComponent: Weapon is null - CRITICAL"));
			return; // Weapon�� ������ �� �̻� �������� ����
		}
	}

	// 5�ܰ�: AnimInstance �ʱ�ȭ
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

	// BeginPlay������ OwnerCharacter�� �����ϰ�, �������� ���߿� �ʱ�ȭ
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ASRCharacter>(GetOwner());
		if (!IsValid(OwnerCharacter))
		{
			UE_LOG(LogTemp, Error, TEXT("CombatActorComponent: Failed to get OwnerCharacter"));
			return;
		}
	}

	// WeaponReady ��������Ʈ�� ���ε�
	if (OwnerCharacter)
	{
		OwnerCharacter->OnWeaponReady.AddDynamic(this, &UCombatActorComponent::OnWeaponReady);
		UE_LOG(LogTemp, Warning, TEXT("CombatActorComponent: Bound to OnWeaponReady delegate"));
	}

	UE_LOG(LogTemp, Warning, TEXT("CombatActorComponent: BeginPlay Finished"));

	// InputBuffer ������Ʈ ã��
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

	// ����: ������ ���� ��� ���⿡ �߰�
	DOREPLIFETIME(UCombatActorComponent, bIsAttacking);
	DOREPLIFETIME(UCombatActorComponent, bIsGuarding);
	DOREPLIFETIME(UCombatActorComponent, bIsParrying);
	DOREPLIFETIME(UCombatActorComponent, bSaveAttack);     // �߰�
	DOREPLIFETIME(UCombatActorComponent, ComboIndex);     // �߰�
	//DOREPLIFETIME(ASRCharacter, bIsStunned);
	// ���⿡ Replicated ���� �߰�
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

	// ���� �߿��� ����/�и��� ĵ�� ����
	if (bIsAttacking && TimeSinceLastAction <= AttackCancelWindow)
	{
		if ((NewAction == TEXT("Guard") && bAllowCancelIntoGuard) ||
			(NewAction == TEXT("Parry") && bAllowCancelIntoParry))
		{
			return true;
		}
	}

	// ���� �߿��� ����/�и��� ��� ��ȯ ����
	if (bIsGuarding)
	{
		if (NewAction == TEXT("Attack") || NewAction == TEXT("Parry"))
		{
			return true;
		}
	}

	// �и� �߿��� ����/����� ��� ��ȯ ����
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

	// �޺� ���� �߿��� InputBuffer ó�� ���� (���� �ý��� �켱)
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
	// ���� �ִϸ��̼� ��� �ߴ�
	TryCancelGuard();
	TryCancelParry();

	if (bIsAttacking)
	{
		// ���� ���̸� ��Ÿ�� �ߴ� �� ���ο� ����
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
			// ��Ÿ�ָ� ���ߴ� �Ͱ� ���ÿ�, �޺� ���� ������ ���⼭ ���� �����ع����ϴ�.
			// ��������Ʈ�� ȣ��Ǳ⸦ ��ٸ��� �ʰ� �ɵ������� ó���ϴ� ���Դϴ�.
			OwnerCharacter->SetCharacterState(ECharacterState::Idle);
			CurrentAnimInstance->Montage_Stop(0.1f, Weapon->AttackMontage);
			ResetCombo(); // ResetCombo()�� ���� ȣ��
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
			// ��Ÿ�ָ� ���ߴ� �Ͱ� ���ÿ�, �޺� ���� ������ ���⼭ ���� �����ع����ϴ�.
			// ��������Ʈ�� ȣ��Ǳ⸦ ��ٸ��� �ʰ� �ɵ������� ó���ϴ� ���Դϴ�.
			OwnerCharacter->SetCharacterState(ECharacterState::Idle);
			CurrentAnimInstance->Montage_Stop(0.1f, Weapon->GuardMontage);
			bIsGuarding = false; // bIsGuard�� ���� ȣ��
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
			// ��Ÿ�ָ� ���ߴ� �Ͱ� ���ÿ�, �޺� ���� ������ ���⼭ ���� �����ع����ϴ�.
			// ��������Ʈ�� ȣ��Ǳ⸦ ��ٸ��� �ʰ� �ɵ������� ó���ϴ� ���Դϴ�.
			OwnerCharacter->SetCharacterState(ECharacterState::Idle);
			CurrentAnimInstance->Montage_Stop(0.1f, Weapon->ParryMontage);
			bIsParrying = false; // ResetCombo()�� ���� ȣ��
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
		if (!bSaveAttack)  // �ߺ� ����
		{
			bSaveAttack = true;
			UE_LOG(LogTemp, Warning, TEXT("Combo attack saved"));
		}
		return;
	}

	// ���ο� ���� ����
	bIsAttacking = true;
	ComboIndex = 0;
	bSaveAttack = false;

	UE_LOG(LogTemp, Warning, TEXT("Starting new attack"));
	Multicast_PlayAttackMontage(0);

}

void UCombatActorComponent::Guard()
{
	// Ŭ���̾�Ʈ�� �ƹ��͵� �˻����� �ʰ� ������ ��û�� �����ϴ�.
	if (!GetOwner()->HasAuthority())
	{
		Server_Guard();
		return;
	}

	// ������� ���� �����մϴ�.
	Server_Guard_Implementation();
}

void UCombatActorComponent::Parry()
{
	// Ŭ���̾�Ʈ�� �ƹ��͵� �˻����� �ʰ� ������ ��û�� �����ϴ�.
	if (!GetOwner()->HasAuthority())
	{
		Server_Parry();
		return;
	}

	// ������� ���� �����մϴ�.
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

	// ù ��° ���ݿ����� bSaveAttack�� false���� ����
	if (ComboIndex == 0 && !bSaveAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("First attack completed - not resetting"));
		return; // ù ��° ������ �������� ����
	}

	if (!bSaveAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("No saved attack - resetting combo"));
		ResetCombo();
		return;
	}

	// ���� �޺� ����
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

	// 2. ��� ���� Weapon, AnimInstance ���, �Լ� ������ ���� ������ ����մϴ�.
	AWeapon* CurrentWeapon = OwnerCharacter->GetEquippedWeapon();
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	if (!IsValid(CurrentWeapon) || !IsValid(CurrentWeapon->GuardMontage) || !IsValid(CurrentAnimInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid components for montage play on %s"), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client"));
		return;
	}

	// ���� ��� ���� ��Ÿ�� ����
	if (CurrentAnimInstance->IsAnyMontagePlaying())
	{
		CurrentAnimInstance->StopAllMontages(0.1f);
	}

	CurrentAnimInstance->Montage_Play(CurrentWeapon->GuardMontage);
}

void UCombatActorComponent::Server_Guard_Implementation()
{
	if (!IsValid(OwnerCharacter)) return;

	// ���������� OwnerCharacter�� ���� ��ȿ�� Weapon�� ������ �� �ֽ��ϴ�.
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

	// 2. ��� ���� Weapon, AnimInstance ���, �Լ� ������ ���� ������ ����մϴ�.
	AWeapon* CurrentWeapon = OwnerCharacter->GetEquippedWeapon();
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	if (!IsValid(CurrentWeapon) || !IsValid(CurrentWeapon->ParryMontage) || !IsValid(CurrentAnimInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid components for montage play on %s"), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client"));
		return;
	}

	// ���� ��� ���� ��Ÿ�� ����
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

	// 2. ��� ���� Weapon, AnimInstance ���, �Լ� ������ ���� ������ ����մϴ�.
	AWeapon* CurrentWeapon = OwnerCharacter->GetEquippedWeapon();
	UAnimInstance* CurrentAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	if (!IsValid(CurrentWeapon) || !IsValid(CurrentWeapon->AttackMontage) || !IsValid(CurrentAnimInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid components for montage play on %s"), GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client"));
		return;
	}

	// ���� ��� ���� ��Ÿ�� ����
	if (CurrentAnimInstance->IsAnyMontagePlaying())
	{
		CurrentAnimInstance->StopAllMontages(0.1f);
	}

	// �޺� �ε����� ���� ���� �̸�
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

	// ��Ÿ�� ���
	float AttackSpeedMultiplier = GetOwnerAttackSpeedMultiplier();
	float PlayLength = CurrentAnimInstance->Montage_Play(CurrentWeapon->AttackMontage, AttackSpeedMultiplier);

	if (PlayLength > 0.0f)
	{
		// �������� ����
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
	// �߰� ������ �˻�
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

	// ���������� ����Ǵ� ���� ���� ����
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
