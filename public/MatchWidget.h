// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyGameInstance.h"
#include "MatchWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API UMatchWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StartMenuWidgetClass;

	void UpdateMatchTime(int SecondsRemaining);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MatchTimeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* IsMatchingText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* CreateRoomButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* MatchButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;


	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnMatchClicked();

	UFUNCTION()
	void OnCreateRoomClicked();

	UFUNCTION()
	void OnCancelClicked();

	float TimeCheck();

	bool bIsMatching = false;
	float MatchingTime;
	APlayerController* PC;
	
	UMyGameInstance* GI;
};
