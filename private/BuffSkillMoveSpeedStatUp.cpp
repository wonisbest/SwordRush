// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSkillMoveSpeedStatUp.h"
#include "SRCharacter.h"
#include "PlayerStats.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

void UBuffSkillMoveSpeedStatUp::ActivateSkill(ASRCharacter* Character)
{
	Super::ActivateSkill(Character);

	BeforeBuffStat = Character->PlayerStat->MoveSpeed;
	AfterBuffStat = BeforeBuffStat * 2;
	Character->PlayerStat->MoveSpeed = AfterBuffStat;
	UKismetSystemLibrary::PrintString(this, TEXT("MoveSpeed Up"), true, true, FLinearColor::Blue, 3.0f);
	Character->GetWorldTimerManager().SetTimer(BuffDurationHandle, FTimerDelegate::CreateUObject(this, &UBuffSkillMoveSpeedStatUp::RevertBuff, Character), 10.0f, false);

}

void UBuffSkillMoveSpeedStatUp::RevertBuff(ASRCharacter* Character)
{
	if (!Character || !Character->PlayerStat) return;

	UKismetSystemLibrary::PrintString(this, TEXT("MoveSpeed Down"), true, true, FLinearColor::Blue, 3.0f);
	Character->PlayerStat->MoveSpeed = BeforeBuffStat;
}