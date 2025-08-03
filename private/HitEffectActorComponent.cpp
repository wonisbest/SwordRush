// Fill out your copyright notice in the Description page of Project Settings.


#include "HitEffectActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SRCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Particles/ParticleSystem.h"  // UParticleSystem 사용을 위해 필요
#include "Camera/CameraShakeBase.h"
#include "GameFramework/PlayerController.h"
#include "DamageFlashComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"


void UHitEffectActorComponent::PlayHitEffects(float DamageAmount, FVector HitLocation)
{
    UE_LOG(LogTemp, Warning, TEXT("PlayHitEffectsCalled"));
    // 검색 결과[1]에 따른 사운드 재생
    if (IsValid(HitSound))
    {
        UGameplayStatics::PlaySound2D(GetWorld(), HitSound, 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
        UE_LOG(LogTemp, Warning, TEXT("HitSound PlaySound2D"));
    }

    // 검색 결과[1]에 따른 파티클 이펙트
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

    // 검색 결과[5]에 따른 카메라 쉐이크 (로컬 플레이어만)
    if (ASRCharacter* Character = Cast<ASRCharacter>(GetOwner()))
    {
        if (Character->IsLocallyControlled() && IsValid(HitCameraShake))
        {
            if (APlayerController* PC = Character->GetController<APlayerController>())
            {
                PC->ClientStartCameraShake(HitCameraShake);
            }
        }

        // 검색 결과[2]에 따른 피격 플래시
        if (UDamageFlashComponent* FlashComp = Character->FindComponentByClass<UDamageFlashComponent>())
        {
            FlashComp->TriggerFlash(FLinearColor::Red);
        }
    }

    // 검색 결과[3]에 따른 Hit Stop (경직)
    if (DamageAmount > 20.0f)
    {
        ApplyHitStop(0.1f);
    }
}

void UHitEffectActorComponent::PlayGuardEffects(FVector HitLocation)
{// 가드 사운드
    if (IsValid(GuardSound))
    {
        UGameplayStatics::PlaySound2D(GetWorld(), GuardSound);
    }

    // 가드 파티클 (스파크 효과)
    if (GuardParticle != nullptr)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GuardParticle, HitLocation);
    }

    // 가드 카메라 쉐이크 (약한 진동)
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
{// 패리 사운드 (금속 충돌음)
    if (IsValid(ParrySound))
    {
        UGameplayStatics::PlaySound2D(GetWorld(), ParrySound);
    }

    // 패리 파티클 (강한 스파크)
    if (ParryParticle != nullptr)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ParryParticle, HitLocation);
    }

    // 검색 결과[3]에 따른 패리 카메라 쉐이크 (강한 진동)
    if (ASRCharacter* Character = Cast<ASRCharacter>(GetOwner()))
    {
        if (Character->IsLocallyControlled() && IsValid(ParryCameraShake))
        {
            if (APlayerController* PC = Character->GetController<APlayerController>())
            {
                PC->ClientStartCameraShake(ParryCameraShake);
            }
        }

        // 패리 성공 시 황금색 플래시
        if (UDamageFlashComponent* FlashComp = Character->FindComponentByClass<UDamageFlashComponent>())
        {
            FlashComp->TriggerFlash(FLinearColor::Yellow);
        }
    }

    // 검색 결과[3]에 따른 Hit Stop (패리 경직)
    ApplyHitStop(0.15f);
}

void UHitEffectActorComponent::ApplyHitStop(float Duration)
{
    // 전역 시간 확장으로 Hit Stop 구현
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


