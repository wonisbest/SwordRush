// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSkillAttackSpeedStatUp.h"
#include "SRCharacter.h"
#include "PlayerStats.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

void UBuffSkillAttackSpeedStatUp::ActivateSkill(ASRCharacter* Character)
{
	Super::ActivateSkill(Character);

	BeforeBuffStat = Character->PlayerStat->AttackSpeed;
	AfterBuffStat = BeforeBuffStat * 2;
	Character->PlayerStat->AttackSpeed = AfterBuffStat;
	UKismetSystemLibrary::PrintString(this, TEXT("AttackSpeed Up"), true, true, FLinearColor::Blue, 3.0f);
	Character->GetWorldTimerManager().SetTimer(BuffDurationHandle, FTimerDelegate::CreateUObject(this, &UBuffSkillAttackSpeedStatUp::RevertBuff, Character), 10.0f, false);

}

void UBuffSkillAttackSpeedStatUp::RevertBuff(ASRCharacter* Character)
{
	if (!Character || !Character->PlayerStat) return;

	UKismetSystemLibrary::PrintString(this, TEXT("AttackSpeed Down"), true, true, FLinearColor::Blue, 3.0f);
	Character->PlayerStat->AttackSpeed = BeforeBuffStat;
}