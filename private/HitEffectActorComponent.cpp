// Fill out your copyright notice in the Description page of Project Settings.


#include "HitEffectActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SRCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Particles/ParticleSystem.h"  // UParticleSystem ����� ���� �ʿ�
#include "Camera/CameraShakeBase.h"
#include "GameFramework/PlayerController.h"
#include "DamageFlashComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"


void UHitEffectActorComponent::PlayHitEffects(float DamageAmount, FVector HitLocation)
{
    UE_LOG(LogTemp, Warning, TEXT("PlayHitEffectsCalled"));
    // �˻� ���[1]�� ���� ���� ���
    if (IsValid(HitSound))
    {
        UGameplayStatics::PlaySound2D(GetWorld(), HitSound, 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
        UE_LOG(LogTemp, Warning, TEXT("HitSound PlaySound2D"));
    }

    // �˻� ���[1]�� ���� ��ƼŬ ����Ʈ
    if (HitParticle != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("HitParticle Spawned"));
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            HitParticle,
            HitLocation,
            FRotator::ZeroRotator
        );
    }

    // �˻� ���[5]�� ���� ī�޶� ����ũ (���� �÷��̾)
    if (ASRCharacter* Character = Cast<ASRCharacter>(GetOwner()))
    {
        if (Character->IsLocallyControlled() && IsValid(HitCameraShake))
        {
            if (APlayerController* PC = Character->GetController<APlayerController>())
            {
                PC->ClientStartCameraShake(HitCameraShake);
            }
        }

        // �˻� ���[2]�� ���� �ǰ� �÷���
        if (UDamageFlashComponent* FlashComp = Character->FindComponentByClass<UDamageFlashComponent>())
        {
            FlashComp->TriggerFlash(FLinearColor::Red);
        }
    }

    // �˻� ���[3]�� ���� Hit Stop (����)
    if (DamageAmount > 20.0f)
    {
        ApplyHitStop(0.1f);
    }
}

void UHitEffectActorComponent::PlayGuardEffects(FVector HitLocation)
{// ���� ����
    if (IsValid(GuardSound))
    {
        UGameplayStatics::PlaySound2D(GetWorld(), GuardSound);
    }

    // ���� ��ƼŬ (����ũ ȿ��)
    if (GuardParticle != nullptr)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GuardParticle, HitLocation);
    }

    // ���� ī�޶� ����ũ (���� ����)
    if (ASRCharacter* Character = Cast<ASRCharacter>(GetOwner()))
    {
        if (Character->IsLocallyControlled() && IsValid(GuardCameraShake))
        {
            if (APlayerController* PC = Character->GetController<APlayerController>())
            {
                PC->ClientStartCameraShake(GuardCameraShake);
            }
        }
    }
}

void UHitEffectActorComponent::PlayParryEffects(FVector HitLocation)
{// �и� ���� (�ݼ� �浹��)
    if (IsValid(ParrySound))
    {
        UGameplayStatics::PlaySound2D(GetWorld(), ParrySound);
    }

    // �и� ��ƼŬ (���� ����ũ)
    if (ParryParticle != nullptr)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryParticle, HitLocation);
    }

    // �˻� ���[3]�� ���� �и� ī�޶� ����ũ (���� ����)
    if (ASRCharacter* Character = Cast<ASRCharacter>(GetOwner()))
    {
        if (Character->IsLocallyControlled() && IsValid(ParryCameraShake))
        {
            if (APlayerController* PC = Character->GetController<APlayerController>())
            {
                PC->ClientStartCameraShake(ParryCameraShake);
            }
        }

        // �и� ���� �� Ȳ�ݻ� �÷���
        if (UDamageFlashComponent* FlashComp = Character->FindComponentByClass<UDamageFlashComponent>())
        {
            FlashComp->TriggerFlash(FLinearColor::Yellow);
        }
    }

    // �˻� ���[3]�� ���� Hit Stop (�и� ����)
    ApplyHitStop(0.15f);
}

void UHitEffectActorComponent::ApplyHitStop(float Duration)
{
    // ���� �ð� Ȯ������ Hit Stop ����
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);

    FTimerHandle HitStopTimer;
    GetWorld()->GetTimerManager().SetTimer(
        HitStopTimer,
        [this]()
        {
            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
        },
        Duration,
        false
    );
}


