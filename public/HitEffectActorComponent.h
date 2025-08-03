// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "HitEffectActorComponent.generated.h"

class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDRUSH_API UHitEffectActorComponent : public UActorComponent
{
	GENERATED_BODY()

	public:
    // �˻� ���[5]�� ���� ī�޶� ����ũ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Effects")
    TSubclassOf<UCameraShakeBase> HitCameraShake;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Effects")
    TSubclassOf<UCameraShakeBase> GuardCameraShake;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Effects")
    TSubclassOf<UCameraShakeBase> ParryCameraShake;

    // �˻� ���[1]�� ���� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* GuardSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* ParrySound;

    // �˻� ���[1]�� ���� ����Ʈ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UNiagaraSystem* HitParticle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UNiagaraSystem* GuardParticle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UNiagaraSystem* ParryParticle;

    UFUNCTION(BlueprintCallable)
    void PlayHitEffects(float DamageAmount, FVector HitLocation);

    UFUNCTION(BlueprintCallable)
    void PlayGuardEffects(FVector HitLocation);

    UFUNCTION(BlueprintCallable)
    void PlayParryEffects(FVector HitLocation);

    UFUNCTION(BlueprintCallable)
    void ApplyHitStop(float Duration);


};
