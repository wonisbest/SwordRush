// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"

void UMatchWidget::NativeConstruct()
{

	Super::NativeConstruct();
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;
	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	MatchingTime = 0.0f;
	if (MatchButton) MatchButton->OnClicked.AddDynamic(this, &UMatchWidget::OnMatchClicked);
	if (CreateRoomButton) CreateRoomButton->OnClicked.AddDynamic(this, &UMatchWidget::OnCreateRoomClicked);
	if (CancelButton) CancelButton->OnClicked.AddDynamic(this, &UMatchWidget::OnCancelClicked);
	if (BackButton) BackButton->OnClicked.AddDynamic(this, &UMatchWidget::OnBackClicked);
	
}

void UMatchWidget::UpdateMatchTime(int SecondsRemaining)
{
	if (MatchTimeText)
	{
		int32 Miniutes = SecondsRemaining / 60;
		int32 Seconds = SecondsRemaining % 60;
		FString TimeStr = FString::Printf(TEXT("%02d:%02d"), Miniutes, Seconds);
		MatchTimeText->SetText(FText::FromString(TimeStr));
	}
	if (IsMatchingText)
	{
		if (bIsMatching)
		{
			FString MathcingStr = FString::Printf(TEXT("MATCHING"));
			IsMatchingText->SetText(FText::FromString(MathcingStr));
		}
		else
		{
			FString MathcingStr = FString::Printf(TEXT("If you want to Match, Please Click the MatchButton"));
			IsMatchingText->SetText(FText::FromString(MathcingStr));
		}
	}
}

void UMatchWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsMatching)
	{
		MatchingTime += InDeltaTime;

		UpdateMatchTime(MatchingTime);
	}
}






void UMatchWidget::OnBackClicked()
{
	RemoveFromParent();

	if (StartMenuWidgetClass)
	{
		UUserWidget* Lobby = CreateWidget<UUserWidget>(GetWorld(), StartMenuWidgetClass);
		if (Lobby)
		{
			Lobby->AddToViewport();
		}
	}
}

void UMatchWidget::OnMatchClicked()
{
	if (!bIsMatching)
	{
		bIsMatching = true;
		MatchingTime = 0.0f;
	}
	//GI->FindAdvancedGameSessions();
	//APlayerController* PC = GetWorld()->GetFirstPlayerController();
	//PC->ClientTravel(ServerAdress, TRAVEL_Absolute);

}

void UMatchWidget::OnCreateRoomClicked()
{

	//FInputModeGameOnly InputModeData;
	//PC->SetInputMode(InputModeData);
	//PC->bShowMouseCursor = false;
	//UGameplayStatics::OpenLevel(this, FName("Level_Match"),true, "listen");
	//GI->CreateAdvancedGameSession();
}

void UMatchWidget::OnCancelClicked()
{
	if(bIsMatching)
	{
		bIsMatching = false;
		MatchingTime = 0.0f;
		UpdateMatchTime(MatchingTime);
	}
}

float UMatchWidget::TimeCheck()
{
	return MatchingTime;
}
