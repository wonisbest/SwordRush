// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStats.h"
#include "SRCharacter.h"
#include "StatsWidget.generated.h"

class UButton;
class UPlayerStats;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API UStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY()
	UPlayerStats* Stats;
	ASRCharacter* SRChar;
	void UpdateStatText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WeaponSkillWidgetClass;
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* AttackUpButton;
	UPROPERTY(meta = (BindWidget))
	UButton* AttackDownButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DefenseUpButton;
	UPROPERTY(meta = (BindWidget))
	UButton* DefenseDownButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AttackSpeedUpButton;
	UPROPERTY(meta = (BindWidget))
	UButton* AttackSpeedDownButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MoveSpeedUpButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MoveSpeedDownButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* WeaponSkillButton;

	UPROPERTY(meta = (BindWidget))
	UButton* StatResetButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextAttack;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextDefense;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextAttackSpeed;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextMoveSpeed;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextStatPoint;

	UFUNCTION()
	void OnAttackUpClicked();

	UFUNCTION()
	void OnAttackDownClicked();

	UFUNCTION()
	void OnDefenseUpClicked();

	UFUNCTION()
	void OnDefenseDownClicked();

	UFUNCTION()
	void OnAttackSpeedUpClicked();

	UFUNCTION()
	void OnAttackSpeedDownClicked();

	UFUNCTION()
	void OnMoveSpeedUpClicked();

	UFUNCTION()
	void OnMoveSpeedDownClicked();

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnSaveClicked();

	UFUNCTION()
	void OnStatResetClicked();

	UFUNCTION()
	void OnWeaponSkillClicked();
};
