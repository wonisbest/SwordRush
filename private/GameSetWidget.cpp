// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSetWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SRPlayerController.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void UGameSetWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC) && !PC->HasAuthority())
	{
		RestartButton->SetVisibility(ESlateVisibility::Hidden);
	}
	if(RestartButton) RestartButton->OnClicked.AddDynamic(this, &UGameSetWidget::OnRestartButtonClick);
	//if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UGameSetWidget::OnExitButtonClick);
}

void UGameSetWidget::OnRestartButtonClick()
{
	// 현재 레벨 재시작
	if (IsValid(PC) && PC->HasAuthority())
	{
		FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
		GetWorld()->ServerTravel(CurrentLevel);
	}
}

void UGameSetWidget::OnExitButtonClick()
{
	//GetWorld()->ServerTravel(FString("Level_First_New"));
}
