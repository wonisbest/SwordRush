// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinSessionsWidget.h"
#include "Components/VerticalBox.h"
#include "SessionGroupWidget.h"
//#include "OnlineSessionSettings.h"

void UJoinSessionsWidget::PopulateSessionList(const TArray<FOnlineSessionSearchResult>& SearchResults)
{
	if (!SessionListBox || !SessionGroupWidgetClass) return;

	for (int32 i = 0; i < SearchResults.Num(); i++)
	{
		USessionGroupWidget* SessionWidget = CreateWidget<USessionGroupWidget>(GetWorld(), SessionGroupWidgetClass);
		if (SessionWidget)
		{
			SessionWidget->SessionIdx = i;

			const auto& Result = SearchResults[i];
			FString SessionName;
			Result.Session.SessionSettings.Get(FName("SESSION_NAME"), SessionName);
			SessionWidget->SessionIdxText->SetText(FText::AsNumber(i));
			SessionWidget->SessionNameText->SetText(FText::FromString(SessionName));
			SessionWidget->SessionPlayerNumberText->SetText(FText::AsNumber(Result.Session.NumOpenPublicConnections));
			SessionWidget->SessionMaxPlayerNumberText->SetText(FText::AsNumber(Result.Session.SessionSettings.NumPublicConnections));

			SessionListBox->AddChild(SessionWidget);
		}
	}
}

void UJoinSessionsWidget::OnFindClicked()
{
}

void UJoinSessionsWidget::OnBackClicked()
{
	RemoveFromParent();
}
