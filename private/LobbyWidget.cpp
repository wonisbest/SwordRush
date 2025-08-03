// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Blueprint/UserWidget.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	if (StartButton) StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnStartClicked);
	if (StatsButton) StatsButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnStatsClicked);
	if (CustomizeButton) CustomizeButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnCustomizeClicked);
	if (OptionButton) OptionButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnOptionClicked);
	if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnExitClicked);
}

void ULobbyWidget::OnStartClicked()
{
	if (StartWidgetClass)
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), StartWidgetClass);
		if (NewWidget)
		{
			RemoveFromParent();
			NewWidget->AddToViewport();
		}
	}

	//FInputModeGameOnly InputModeData;
	//PC->SetInputMode(InputModeData);
	//PC->bShowMouseCursor = false;
	//UGameplayStatics::OpenLevel(GetWorld(), FName("Level_Traning"));
	
}

void ULobbyWidget::OnStatsClicked()
{
	if (StatsWidgetClass)
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), StatsWidgetClass);
		if (NewWidget)
		{
			RemoveFromParent();
			NewWidget->AddToViewport();
		}
	}
}

void ULobbyWidget::OnCustomizeClicked()
{
	if (CustomizeWidgetClass)
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), CustomizeWidgetClass);
		if (NewWidget)
		{
			RemoveFromParent();
			NewWidget->AddToViewport();
		}
	}
}

void ULobbyWidget::OnOptionClicked()
{
	if (OptionWidgetClass)
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), OptionWidgetClass);
		if (NewWidget)
		{
			RemoveFromParent();
			NewWidget->AddToViewport();
		}
	}
}

void ULobbyWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, true);
}
