// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSkillAttackStatUp.h"
#include "SRCharacter.h"
#include "PlayerStats.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

void UBuffSkillAttackStatUp::ActivateSkill(ASRCharacter* Character)
{
	Super::ActivateSkill(Character);

	BeforeBuffStat = Character->PlayerStat->Attack;
	AfterBuffStat = BeforeBuffStat * 2;
	Character->PlayerStat->Attack = AfterBuffStat;
	UKismetSystemLibrary::PrintString(this, TEXT("Attack Up"), true, true, FLinearColor::Blue, 3.0f);
	Character->GetWorldTimerManager().SetTimer(BuffDurationHandle, FTimerDelegate::CreateUObject(this, &UBuffSkillAttackStatUp::RevertBuff, Character), 10.0f, false);

}

void UBuffSkillAttackStatUp::RevertBuff(ASRCharacter* Character)
{
	if (!Character || !Character->PlayerStat) return;

	UKismetSystemLibrary::PrintString(this, TEXT("Attack Down"), true, true, FLinearColor::Blue, 3.0f);
	Character->PlayerStat->Attack = BeforeBuffStat;
}
