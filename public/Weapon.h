// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UCapsuleComponent;
class ASRCharacter;
UCLASS()
class SWORDRUSH_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// �⺻ ���� ������ (PlayerStat�� ���� �� ���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseDamage = 20.0f;

	// ���� ������ ���� (���⺰ ����ȭ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageMultiplier = 1.0f;

	// �̹� Ÿ���� ���͵��� ���� (�ߺ� Ÿ�� ����)
	UPROPERTY()
	TArray<AActor*> HitActors;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "WeaponComponents")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "WeaponComponents")
	UCapsuleComponent* HitCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Combat")
	UAnimMontage* GuardMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Combat")
	UAnimMontage* ParryMontage;

	UFUNCTION()
	void EnableHit();

	UFUNCTION()
	void DisableHit();

	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// ���� ���� �� ȣ�� (��Ʈ ���� ����Ʈ �ʱ�ȭ)
	UFUNCTION(BlueprintCallable)
	void StartAttack();

	// ���� ���� �� ȣ��
	UFUNCTION(BlueprintCallable)
	void EndAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponComponents")
	FName WeaponName;

	//UFUNCTION(Server, Reliable)
	//void Server_ApplyDamage(ASRCharacter* Target, ASRCharacter* Attacker);

	// �˻� ���[1]�� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* AttackSound;
private:
	// ���� ������ ��� �� ����
	void ApplyDamageToTarget(ASRCharacter* Target);
};
