// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSetWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API UGameSetWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameResultText;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void OnRestartButtonClick();

	UFUNCTION()
	void OnExitButtonClick();

	APlayerController* PC;
};
