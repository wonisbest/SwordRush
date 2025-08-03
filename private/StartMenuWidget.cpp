// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UStartMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MatchButton) MatchButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnMatchButtonClicked);
	if (InviteButton) InviteButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnInviteButtonClicked);
	if (TrainingButton) TrainingButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnTrainingButtonClicked);
	if (BackButton) BackButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnBackButtonClicked);
}

void UStartMenuWidget::OnMatchButtonClicked()
{
	RemoveFromParent();

	if (MatchWidgetClass)
	{
		UUserWidget* Lobby = CreateWidget<UUserWidget>(GetWorld(), MatchWidgetClass);
		if (Lobby)
		{
			Lobby->AddToViewport();
		}
	}
}

void UStartMenuWidget::OnInviteButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level_Lobby"));
}

void UStartMenuWidget::OnTrainingButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level_Training"));
}

void UStartMenuWidget::OnBackButtonClicked()
{
	RemoveFromParent();

	if (LobbyWidgetClass)
	{
		UUserWidget* Lobby = CreateWidget<UUserWidget>(GetWorld(), LobbyWidgetClass);
		if (Lobby)
		{
			Lobby->AddToViewport();
		}
	}
}
