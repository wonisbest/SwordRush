// Fill out your copyright notice in the Description page of Project Settings.


#include "SKillCoolDownManager.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

void USKillCoolDownManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USKillCoolDownManager, CoolDowns);
}

void USKillCoolDownManager::StartCoolDown(FName SkillName, float CoolDownDuration)
{
    if (CoolDownDuration <= 0.f || SkillName.IsNone() || !IsValid(this)) return;

    if (UWorld* World = GetWorld())
    {
        float EndTime = World->GetTimeSeconds() + CoolDownDuration;
        for (FCoolDownInfo& Info : CoolDowns)
        {
            if (Info.SkillName == SkillName)
            {
                Info.EndTime = EndTime;
                return;
            }
        }
        CoolDowns.Add({ SkillName, EndTime });
    }
}

bool USKillCoolDownManager::IsOnCoolDown(FName SkillName) const
{
    // 강화된 안전성 검사
    if (!IsValid(this) || SkillName.IsNone())
    {
        return false;
    }

    for (const FCoolDownInfo& Info : CoolDowns)
    {
        if (Info.SkillName == SkillName)
        {
            if (UWorld* World = GetWorld())
            {
                return World->GetTimeSeconds() < Info.EndTime;
            }
        }
    }
    return false;
}

float USKillCoolDownManager::GetRemainingCoolDown(FName SkillName) const
{
    if (!IsValid(this) || SkillName.IsNone()) return 0.0f;

    for (const FCoolDownInfo& Info : CoolDowns)
    {
        if (Info.SkillName == SkillName)
        {
            if (UWorld* World = GetWorld())
            {
                return FMath::Max(Info.EndTime - World->GetTimeSeconds(), 0.0f);
            }
        }
    }
    return 0.0f;
}