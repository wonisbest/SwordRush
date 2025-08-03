// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPartIndex
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 HatIndex;

	UPROPERTY(BlueprintReadWrite)
	int32 EyeIndex;
	
	UPROPERTY(BlueprintReadWrite)
	int32 FaceIndex;

	UPROPERTY(BlueprintReadWrite)
	int32 NoseIndex;

	UPROPERTY(BlueprintReadWrite)
	int32 TopIndex;

	UPROPERTY(BlueprintReadWrite)
	int32 BottomIndex;

	UPROPERTY(BlueprintReadWrite)
	int32 GlovesIndex;

	UPROPERTY(BlueprintReadWrite)
	int32 ShoesIndex;
};

USTRUCT(BlueprintType)
struct FSRPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 AttackPoint = 1 ;

	UPROPERTY(BlueprintReadWrite)
	int32 DeffensePoint = 1;

	UPROPERTY(BlueprintReadWrite)
	int32 AttackSpeedPoint = 1;

	UPROPERTY(BlueprintReadWrite)
	int32 MoveSpeedPoint = 1;

	UPROPERTY(BlueprintReadWrite)
	int32 StatPoint = 15;
};

USTRUCT(BlueprintType)
struct FWeaponSet
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	int32 WeaponIndex;
};

USTRUCT(BlueprintType)
struct FSkillSet
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	int32 SkillIndex;
};

USTRUCT(BlueprintType)
struct FOptionSettings
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FIntPoint ScreenResolution = FIntPoint(1920, 1080);

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EWindowMode::Type> FullscreenMode = EWindowMode::Fullscreen;

	UPROPERTY(BlueprintReadWrite)
	int32 QualityLevel = 3;

	UPROPERTY(BlueprintReadWrite)
	float MasterVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float MusicVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float SFXVolume = 1.0f;
};

UCLASS()
class SWORDRUSH_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FPartIndex CustomizePartIndex;

	UPROPERTY(BlueprintReadWrite)
	FSRPlayerStats StatValues;

	UPROPERTY(BlueprintReadWrite)
	FWeaponSet WeaponSetIndex;

	UPROPERTY(BlueprintReadWrite)
	FSkillSet SkillSetIndex;

	UPROPERTY(BlueprintReadWrite)
	FOptionSettings OptionSettings;
};
