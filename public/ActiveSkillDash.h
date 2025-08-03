// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveSkill.h"
#include "ActiveSkillDash.generated.h"

/**
 * 
 */
UCLASS()
class SWORDRUSH_API UActiveSkillDash : public UActiveSkill
{
	GENERATED_BODY()
	
public:
	virtual void ActivateSkill(class ASRCharacter* Character) override;

private:
	void HandleDash(class ASRCharacter* Character);

	FVector StartLocation;
	FVector Direction;
	float DashDistance;
	float DashTime;
	float ElapsedTime;

	

	FTimerHandle DashTimerHandle;

};
