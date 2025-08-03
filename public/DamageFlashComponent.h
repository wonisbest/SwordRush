// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageFlashComponent.generated.h"

UCLASS()
class SWORDRUSH_API UDamageFlashComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UDamageFlashComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash")
    FLinearColor FlashColor = FLinearColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash")
    float FlashDuration = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash")
    UMaterialInterface* FlashMaterial;

    UFUNCTION(BlueprintCallable)
    void TriggerFlash(FLinearColor Color = FLinearColor::Red);

private:
    UPROPERTY()
    UMaterialInstanceDynamic* FlashMaterialInstance;

    FTimerHandle FlashTimerHandle;

    void CreateFlashMaterial();
    void FadeOutFlash();

};
