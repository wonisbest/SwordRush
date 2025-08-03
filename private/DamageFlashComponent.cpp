// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageFlashComponent.h"
#include "SRCharacter.h"

// Sets default values for this component's properties
UDamageFlashComponent::UDamageFlashComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
}

void UDamageFlashComponent::TriggerFlash(FLinearColor Color)
{
    // 검색 결과[2]에 따른 구현
    if (!FlashMaterialInstance)
    {
        CreateFlashMaterial();
    }

    if (FlashMaterialInstance)
    {
        // 플래시 색상 및 투명도 설정
        FlashMaterialInstance->SetVectorParameterValue(TEXT("FlashColor"), Color);
        FlashMaterialInstance->SetScalarParameterValue(TEXT("Opacity"), 1.0f);

        // 타이머로 페이드 아웃
        GetWorld()->GetTimerManager().SetTimer(
            FlashTimerHandle,
            this,
            &UDamageFlashComponent::FadeOutFlash,
            FlashDuration,
            false
        );
    }
}

void UDamageFlashComponent::CreateFlashMaterial()
{
    if (FlashMaterial && GetOwner())
    {
        if (ASRCharacter* Character = Cast<ASRCharacter>(GetOwner()))
        {
            FlashMaterialInstance = UMaterialInstanceDynamic::Create(FlashMaterial, this);
            // 캐릭터 메시에 적용
            Character->GetMesh()->SetOverlayMaterial(FlashMaterialInstance);
        }
    }
}

void UDamageFlashComponent::FadeOutFlash()
{
    if (FlashMaterialInstance)
    {
        FlashMaterialInstance->SetScalarParameterValue(TEXT("Opacity"), 0.0f);
    }
}