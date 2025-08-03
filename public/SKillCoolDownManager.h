// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SKillCoolDownManager.generated.h"

/**
 * 
 */
USTRUCT()
struct FCoolDownInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FName SkillName;

	UPROPERTY()
	float EndTime;
};

UCLASS(Blueprintable)
class SWORDRUSH_API USKillCoolDownManager : public UObject
{
	GENERATED_BODY()
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual bool IsSupportedForNetworking() const override { return true; }

	UFUNCTION()
	void StartCoolDown(FName SkillName, float CoolDownDuration);

	UFUNCTION()
	bool IsOnCoolDown(FName SKillName) const;

	UFUNCTION()
	float GetRemainingCoolDown(FName SkillName) const;

private:
	UPROPERTY(Replicated)
	TArray<FCoolDownInfo> CoolDowns;  // TSet<TTuple<FName,float>> 대신 사용
};
