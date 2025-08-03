// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skill.generated.h"

class ASRCharacter;
/**
 * 
 */
UCLASS(Blueprintable)
class SWORDRUSH_API USkill : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FText SkillDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	UTexture2D* SKillIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SKill")
	UAnimMontage* SkillMontage;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 SkillLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CurrentCoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bIsSkillActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* SkillSound;

	USkill();

	virtual void ActivateSkill(class ASRCharacter* Character);
};
