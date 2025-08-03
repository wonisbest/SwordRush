// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API UStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MatchWidgetClass;
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* MatchButton;

	UPROPERTY(meta = (BindWidget))
	UButton* InviteButton;

	UPROPERTY(meta = (BindWidget))
	UButton* TrainingButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UFUNCTION()
	void OnMatchButtonClicked();

	UFUNCTION()
	void OnInviteButtonClicked();

	UFUNCTION()
	void OnTrainingButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

};
