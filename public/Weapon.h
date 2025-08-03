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

	// 기본 무기 데미지 (PlayerStat이 없을 때 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseDamage = 20.0f;

	// 무기 데미지 배율 (무기별 차별화)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageMultiplier = 1.0f;

	// 이미 타격한 액터들을 저장 (중복 타격 방지)
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

	// 공격 시작 시 호출 (히트 액터 리스트 초기화)
	UFUNCTION(BlueprintCallable)
	void StartAttack();

	// 공격 종료 시 호출
	UFUNCTION(BlueprintCallable)
	void EndAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponComponents")
	FName WeaponName;

	//UFUNCTION(Server, Reliable)
	//void Server_ApplyDamage(ASRCharacter* Target, ASRCharacter* Attacker);

	// 검색 결과[1]에 따른 사운드 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* AttackSound;
private:
	// 실제 데미지 계산 및 적용
	void ApplyDamageToTarget(ASRCharacter* Target);
};
