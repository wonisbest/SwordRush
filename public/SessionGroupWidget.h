// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionGroupWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API USessionGroupWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;


public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionIdxText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionPlayerNumberText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionMaxPlayerNumberText;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	int32 SessionIdx;

	UFUNCTION()
	void OnJoinClicked();

};
