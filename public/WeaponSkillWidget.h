// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRCharacter.h"
#include "Weapon.h"
#include "WeaponSkillWidget.generated.h"

class UButton;
class UPlayerStats;
class UTextBlock;

/**
 * 
 */
UCLASS()
class SWORDRUSH_API UWeaponSkillWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	ASRCharacter* SRChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StatsWidgetClass;

	int32 UI_WeaponIdx;
	int32 UI_SkillIdx;
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* WeaponPrevButton;

	UPROPERTY(meta = (BindWidget))
	UButton* WeaponNextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SkillPrevButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SkillNextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillExplainTextBlock;

	UFUNCTION()
	void OnWeaponPrevClicked();

	UFUNCTION()
	void OnWeaponNextClicked();

	UFUNCTION()
	void OnSkillPrevClicked();

	UFUNCTION()
	void OnSkillNextClicked();

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnSaveClicked();

	UFUNCTION()
	void SetWeaponNameText();

	UFUNCTION()
	void SetSkillNameText();

	UFUNCTION()
	void SetSkillExplainText();
};
