// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skill.h"
#include "SkillList.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SWORDRUSH_API USkillList : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<USkill>> Skills;
};
