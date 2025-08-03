// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSkillDefenseStatUp.h"
#include "SRCharacter.h"
#include "PlayerStats.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

void UBuffSkillDefenseStatUp::ActivateSkill(ASRCharacter* Character)
{
	Super::ActivateSkill(Character);

	BeforeBuffStat = Character->PlayerStat->Defense;
	AfterBuffStat = BeforeBuffStat * 2;
	Character->PlayerStat->Defense = AfterBuffStat;
	UKismetSystemLibrary::PrintString(this, TEXT("Defense Up"), true, true, FLinearColor::Blue, 3.0f);
	Character->GetWorldTimerManager().SetTimer(BuffDurationHandle, FTimerDelegate::CreateUObject(this, &UBuffSkillDefenseStatUp::RevertBuff, Character), 10.0f, false);

}

void UBuffSkillDefenseStatUp::RevertBuff(ASRCharacter* Character)
{
	if (!Character || !Character->PlayerStat) return;

	UKismetSystemLibrary::PrintString(this, TEXT("Defense Down"), true, true, FLinearColor::Blue, 3.0f);
	Character->PlayerStat->Defense = BeforeBuffStat;
}