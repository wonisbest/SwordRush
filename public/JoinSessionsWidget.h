// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionGroupWidget.h"
#include "Components/TextBlock.h"
#include "OnlineSessionSettings.h"
#include "JoinSessionsWidget.generated.h"

class UButton;
class UVerticalBox;
class USessionGroupWidget;
//struct FOnlineSessionSearchResult;
/**
 * 
 */

UCLASS()
class SWORDRUSH_API UJoinSessionsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* SessionListBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Session")
	TSubclassOf<USessionGroupWidget> SessionGroupWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UButton* FindButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;


	void PopulateSessionList(const TArray<FOnlineSessionSearchResult>& SearchResults);

	UFUNCTION()
	void OnFindClicked();

	UFUNCTION()
	void OnBackClicked();
};
