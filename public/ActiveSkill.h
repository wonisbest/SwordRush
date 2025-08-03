// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "ActiveSkill.generated.h"

/**
 * 
 */
UCLASS()
class SWORDRUSH_API UActiveSkill : public USkill
{
	GENERATED_BODY()
	
public:
	virtual void ActivateSkill(ASRCharacter* Character) override;
};
