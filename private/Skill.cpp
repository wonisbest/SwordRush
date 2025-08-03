// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill.h"
#include "Kismet/GameplayStatics.h"
#include "SRCharacter.h"

USkill::USkill()
{
	
}

void USkill::ActivateSkill(ASRCharacter* Character)
{
    UE_LOG(LogTemp, Warning, TEXT("ActivateSkill called"));

    if (!IsValid(Character))
    {
        UE_LOG(LogTemp, Error, TEXT("Character is NULL"));
        return;
    }

    if (!IsValid(SkillSound))
    {
        UE_LOG(LogTemp, Error, TEXT("SkillSound is NULL"));
        return;
    }

    UWorld* World = Character->GetWorld();
    if (!IsValid(World))
    {
        UE_LOG(LogTemp, Error, TEXT("World is NULL"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Playing sound: %s"), *SkillSound->GetName());
    UGameplayStatics::PlaySound2D(World, SkillSound);
}
	
