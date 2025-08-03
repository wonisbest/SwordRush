// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserInterfaceWidget.generated.h"

class UButton;
class USizeBox;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API ULobbyUserInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// Social Widget Ŭ���� ���� (TSubclassOf ���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Widget")
	TSubclassOf<class USessionWidget> SessionWidgetClass;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* SocialButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ReadyButton;

	UPROPERTY(meta = (BindWidget))
	USizeBox* SessionParentSizeBox;

	UPROPERTY()
	USessionWidget* CurrentSessionWidget;

	// ��ư Ŭ�� �̺�Ʈ
	UFUNCTION()
	void OnSocialButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnReadyButtonClicked();

	bool bIsSocialOn = false;
};
