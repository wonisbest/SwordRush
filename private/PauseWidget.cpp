// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	if (StartButton) StartButton->OnClicked.AddDynamic(this, &UPauseWidget::OnStartClicked);
	if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnExitClicked);
}

void UPauseWidget::OnStartClicked()
{

}

void UPauseWidget::OnExitClicked()
{
	RemoveFromParent();
    RemoveFromParent();

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            // 델리게이트 바인딩
            OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPauseWidget::OnDestroySessionComplete);
            DestroySessionDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

            // 세션 파괴 요청
            SessionInterface->DestroySession(NAME_GameSession);
            return; // 세션 파괴 완료 후 레벨 이동
        }
    }

    // 세션 시스템이 없으면 바로 레벨 이동
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level_First_New"));
	
}

void UPauseWidget::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    // 델리게이트 해제
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
        }
    }

    // 레벨 이동
    UGameplayStatics::OpenLevel(GetWorld(), FName("Level_First_New"));
}
