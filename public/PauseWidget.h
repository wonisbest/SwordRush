// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PauseWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnExitClicked();

	APlayerController* PC;

	// 델리게이트 및 핸들
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionDelegateHandle;

	// 콜백 함수 선언
	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};
