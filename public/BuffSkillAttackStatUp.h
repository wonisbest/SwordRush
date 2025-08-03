// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffSkill.h"
#include "BuffSkillAttackStatUp.generated.h"

/**
 * 
 */
UCLASS()
class SWORDRUSH_API UBuffSkillAttackStatUp : public UBuffSkill
{
	GENERATED_BODY()
	
public:
	virtual void ActivateSkill(ASRCharacter* Character) override;

	

private:
	virtual void RevertBuff(ASRCharacter* Character) override;
	float BeforeBuffStat;
	float AfterBuffStat;
	FTimerHandle BuffDurationHandle;
};
