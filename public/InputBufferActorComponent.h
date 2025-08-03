// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputBufferActorComponent.generated.h"

// USTRUCT를 클래스 외부에서 선언 (Global scope)
USTRUCT(BlueprintType)
struct FBufferedInput
{
    GENERATED_BODY()

    UPROPERTY()
    float Timestamp = 0.0f;

    UPROPERTY()
    float Priority = 1.0f;

    FBufferedInput()
    {
        Timestamp = 0.0f;
        Priority = 1.0f;
    }

    FBufferedInput(float InTimestamp, float InPriority)
        : Timestamp(InTimestamp), Priority(InPriority)
    {
    }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SWORDRUSH_API UInputBufferActorComponent : public UActorComponent
{
    GENERATED_BODY()

    public:
    UInputBufferActorComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BufferWindow = 0.15f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CancelWindow = 0.3f; // 캔슬 가능한 시간 윈도우

    UFUNCTION(BlueprintCallable)
    void BufferInput(FName InputName, float Priority = 1.0f);

    UFUNCTION(BlueprintCallable)
    bool ConsumeBufferedInput(FName InputName);

    UFUNCTION(BlueprintCallable)
    FName GetHighestPriorityInput();

    UFUNCTION(BlueprintCallable)
    void ClearAllBuffers();

private:
    UPROPERTY()
    TMap<FName, FBufferedInput> BufferedInputs;

    void CleanExpiredInputs();
};
