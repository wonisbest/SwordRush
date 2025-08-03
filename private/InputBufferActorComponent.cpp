// Fill out your copyright notice in the Description page of Project Settings.

#include "InputBufferActorComponent.h"

// Sets default values for this component's properties
UInputBufferActorComponent::UInputBufferActorComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
}

void UInputBufferActorComponent::BufferInput(FName InputName, float Priority)
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    BufferedInputs.Add(InputName, FBufferedInput(CurrentTime, Priority));

    UE_LOG(LogTemp, Log, TEXT("Input buffered: %s (Priority: %.2f)"),
        *InputName.ToString(), Priority);

    CleanExpiredInputs();
}

bool UInputBufferActorComponent::ConsumeBufferedInput(FName InputName)
{
    if (FBufferedInput* InputData = BufferedInputs.Find(InputName))
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        if (CurrentTime - InputData->Timestamp <= BufferWindow)
        {
            BufferedInputs.Remove(InputName);
            return true;
        }
    }
    return false;
}

FName UInputBufferActorComponent::GetHighestPriorityInput()
{
    CleanExpiredInputs();

    FName HighestPriorityInput = NAME_None;
    float HighestPriority = 0.0f;

    for (const auto& InputPair : BufferedInputs)
    {
        if (InputPair.Value.Priority > HighestPriority)
        {
            HighestPriority = InputPair.Value.Priority;
            HighestPriorityInput = InputPair.Key;
        }
    }

    return HighestPriorityInput;
}

void UInputBufferActorComponent::ClearAllBuffers()
{
    BufferedInputs.Empty();
    UE_LOG(LogTemp, Log, TEXT("All input buffers cleared"));
}

void UInputBufferActorComponent::CleanExpiredInputs()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();

    TArray<FName> ExpiredInputs;
    for (const auto& InputPair : BufferedInputs)
    {
        if (CurrentTime - InputPair.Value.Timestamp > BufferWindow)
        {
            ExpiredInputs.Add(InputPair.Key);
        }
    }

    for (const FName& ExpiredInput : ExpiredInputs)
    {
        BufferedInputs.Remove(ExpiredInput);
    }
}
