// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingPlayerWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API UWaitingPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* WaitingAnnouncementTextBlock;

	float CountDownTime;
	void CountDown();

	bool bIsGameReady;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
