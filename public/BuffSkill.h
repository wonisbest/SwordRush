// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "BuffSkill.generated.h"

/**
 * 
 */
UCLASS()
class SWORDRUSH_API UBuffSkill : public USkill
{
	GENERATED_BODY()
	
public:
	virtual void ActivateSkill(ASRCharacter* Character) override;

private:
	virtual void RevertBuff(ASRCharacter* Character);
};
