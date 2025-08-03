// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatActorComponent.generated.h"

class UInputBufferActorComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDRUSH_API UCombatActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatActorComponent();

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsAttacking = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bSaveAttack = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsParrying = false;
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bIsGuarding = false;

	UPROPERTY(Replicated, BlueprintReadOnly,category = "Combat")
	int32 ComboIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* StunMontage;

	void Attack();
	void Guard();
	void Parry();

	UFUNCTION(NetMulticast, Reliable)
	void PlayComboAttack();

	void ResetCombo();

	
	void NextComboAttack();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackMontage(int32 idx);
	UFUNCTION(Server, Reliable)
	void Server_Attack();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayGuardMontage();
	UFUNCTION(Server, Reliable)
	void Server_Guard();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayParryMontage();
	UFUNCTION(Server, Reliable)
	void Server_Parry();

	UFUNCTION(Server, Reliable)
	void Server_Stun(ASRCharacter* StunChar);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayStunMontage(ASRCharacter* StunChar);

	UFUNCTION()
	void OnWeaponReady(TSubclassOf<AWeapon> NewWeapon);

	UFUNCTION()
	void SetInitailizeCombatComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	UInputBufferActorComponent* InputBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowCancelIntoGuard = true; // 공격 중 가드 캔슬 허용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowCancelIntoParry = true; // 공격 중 패리 캔슬 허용

	UFUNCTION(BlueprintCallable)
	bool CanCancelCurrentAction(FName NewAction);

	UFUNCTION(BlueprintCallable)
	void ProcessBufferedInputs();

	void ForceExecuteAttack();
	void ForceExecuteGuard();
	void ForceExecuteParry();

private:
	UPROPERTY()
	class ASRCharacter* OwnerCharacter;
	UPROPERTY()
	class TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY()
	AWeapon* Weapon;
	UPROPERTY()
	UAnimInstance* AnimInstance;

	void TryCancelAttack();
	void TryCancelGuard();
	void TryCancelParry();

	float GetOwnerAttackSpeedMultiplier() const;

	float LastActionStartTime = 0.0f;

	UPROPERTY(EditAnywhere)
	float AttackCancelWindow = 0.4f; // 공격 시작 후 400ms 동안 캔슬 가능
};
