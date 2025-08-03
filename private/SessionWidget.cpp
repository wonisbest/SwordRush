#include "SessionWidget.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlinePresenceInterface.h" 
#include "Interfaces/OnlineIdentityInterface.h"

void USessionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 안전한 함수 포인터 사용
    FTimerHandle DelayedInitTimer;
    GetWorld()->GetTimerManager().SetTimer(DelayedInitTimer, this, &USessionWidget::DelayedInitialize, 2.0f, false);
}

void USessionWidget::NativeDestruct()
{
    // 타이머 정리
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(RefreshTimerHandle);
    }

    // 버튼 이벤트 정리
    if (RefreshButton)
    {
        RefreshButton->OnClicked.RemoveAll(this);
    }

    // Social Widget들 정리
    ClearSocialWidgets();

    Super::NativeDestruct();
}

void USessionWidget::UpdateFriendsList()
{
    // 기존 Widget들 제거
    ClearSocialWidgets();

    // 친구 목록 정렬
    SortFriends();

    int32 OnlineFriendsCount = 0;
    for (const FFriendInfo& FriendInfo : FriendsList)
    {
        // 매개변수와 함께 호출
        USocialWidget* SocialWidget = CreateSocialWidget(FriendInfo);
        if (SocialWidget)
        {
            // 기존 바인딩 제거 후 새로 바인딩
            SocialWidget->OnFriendInvitedEvent.RemoveAll(this);
            SocialWidget->OnFriendInvitedEvent.AddDynamic(this, &USessionWidget::OnFriendInvited);

            if (FriendsContainer)
            {
                FriendsContainer->AddChild(SocialWidget);
            }

            SocialWidgets.Add(SocialWidget);

            if (FriendInfo.bIsOnline)
            {
                OnlineFriendsCount++;
            }
        }
    }

    OnFriendsListUpdatedEvent.Broadcast(OnlineFriendsCount, FriendsList.Num());
}

void USessionWidget::RefreshFriendsList()
{
    UE_LOG(LogTemp, Warning, TEXT("RefreshFriendsList called"));

    if (!GetWorld() || !GetWorld()->IsGameWorld())
    {
        UE_LOG(LogTemp, Warning, TEXT("SessionWidget: Not in game world, skipping refresh"));
        return;
    }

    // Steam 초기화 상태 확인
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("OnlineSubsystem is null"));
        return;
    }

    // Steam 로그인 상태 확인
    IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
    if (!IdentityInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("IdentityInterface is not valid"));
        return;
    }

    if (IdentityInterface->GetLoginStatus(0) != ELoginStatus::LoggedIn)
    {
        UE_LOG(LogTemp, Error, TEXT("User is not logged in to Steam"));
        return;
    }

    if (!FriendsInterface.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("FriendsInterface not valid, attempting to reinitialize"));
        FriendsInterface = OnlineSubsystem->GetFriendsInterface();

        if (!FriendsInterface.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to reinitialize FriendsInterface"));
            return;
        }
    }

    // 안전한 Steam 친구 목록 읽기
    UE_LOG(LogTemp, Warning, TEXT("Attempting to read Steam friends list..."));
    // 수정된 코드
    FriendsInterface->ReadFriendsList(0, EFriendsLists::ToString(EFriendsLists::Default),
        FOnReadFriendsListComplete::CreateUObject(this, &USessionWidget::OnReadFriendsListComplete));

    UE_LOG(LogTemp, Log, TEXT("SessionWidget: ReadFriendsList called successfully"));
}

void USessionWidget::SortFriends()
{
    // 온라인 친구를 먼저, 그 다음 오프라인 친구 순으로 정렬
    FriendsList.Sort([](const FFriendInfo& A, const FFriendInfo& B)
    {
        if (A.bIsOnline != B.bIsOnline)
        {
            return A.bIsOnline > B.bIsOnline; // 온라인 친구가 먼저
        }
        return A.DisplayName < B.DisplayName; // 이름 순으로 정렬
    });
}

void USessionWidget::OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
    UE_LOG(LogTemp, Warning, TEXT("OnReadFriendsListComplete called - Success: %s"), bWasSuccessful ? TEXT("True") : TEXT("False"));

    if (!bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("SessionWidget: Failed to read friends list - %s"), *ErrorStr);

        // 실패 시 빈 목록으로 UI 업데이트
        FriendsList.Empty();
        UpdateFriendsList();
        return;
    }

    if (!IsValid(this))
    {
        UE_LOG(LogTemp, Error, TEXT("SessionWidget is not valid in callback"));
        return;
    }

    if (!FriendsInterface.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("FriendsInterface is not valid in callback"));
        return;
    }

    if (!bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("SessionWidget: Failed to read friends list - %s"), *ErrorStr);
        return;
    }

    if (!FriendsInterface.IsValid())
    {
        return;
    }

    // 친구 목록 가져오기
    TArray<TSharedRef<FOnlineFriend>> OnlineFriends;
    FriendsInterface->GetFriendsList(LocalUserNum, ListName, OnlineFriends);

    // FFriendInfo로 변환
    FriendsList.Empty();
    for (TSharedRef<FOnlineFriend> OnlineFriend : OnlineFriends)
    {
        FFriendInfo FriendInfo = ConvertToFriendInfo(OnlineFriend);
        FriendsList.Add(FriendInfo);
    }

    // UI 업데이트
    UpdateFriendsList();

    UE_LOG(LogTemp, Log, TEXT("SessionWidget: Successfully loaded %d friends"), FriendsList.Num());
}

USocialWidget* USessionWidget::CreateSocialWidget(const FFriendInfo& FriendInfo)
{
    if (!SocialWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("SessionWidget: SocialWidgetClass is not set"));
        return nullptr;
    }

    // Social Widget 생성
    USocialWidget* SocialWidget = CreateWidget<USocialWidget>(this, SocialWidgetClass);
    if (SocialWidget)
    {
        // 친구 정보 설정
        SocialWidget->UpdateFriendInfo(FriendInfo);
    }

    return SocialWidget;
}

void USessionWidget::ClearSocialWidgets()
{
    for (USocialWidget* SocialWidget : SocialWidgets)
    {
        if (IsValid(SocialWidget))
        {
            SocialWidget->OnFriendInvitedEvent.RemoveAll(this);
            SocialWidget->RemoveFromParent();
            // 명시적으로 null 설정
            SocialWidget = nullptr;
        }
    }
    SocialWidgets.Empty();

    if (FriendsContainer)
    {
        FriendsContainer->ClearChildren();
    }

}

void USessionWidget::OnFriendInvited(const FFriendInfo& FriendInfo)
{
    UE_LOG(LogTemp, Log, TEXT("SessionWidget: Friend invited - %s"), *FriendInfo.DisplayName);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
            FString::Printf(TEXT("Sent game invite to %s"), *FriendInfo.DisplayName));
    }
}

void USessionWidget::OnRefreshButtonClicked()
{
    //UE_LOG(LogTemp, Warning, TEXT("Refresh button clicked"));

    //if (!IsValid(this))
    //{
    //    UE_LOG(LogTemp, Error, TEXT("SessionWidget is not valid"));
    //    return;
    //}

    //if (!FriendsInterface.IsValid())
    //{
    //    UE_LOG(LogTemp, Error, TEXT("FriendsInterface is not initialized, cannot refresh"));
    //    return;
    //}

    //RefreshFriendsList();
}


void USessionWidget::DelayedInitialize()
{
    if (IsValid(this) && GetWorld() && GetWorld()->IsGameWorld())
    {
        InitializeOnlineInterfaces();
    }
}

void USessionWidget::InitializeOnlineInterfaces()
{
    UE_LOG(LogTemp, Warning, TEXT("InitializeOnlineInterfaces called"));

    if (!IsValid(this))
    {
        UE_LOG(LogTemp, Error, TEXT("SessionWidget is not valid in InitializeOnlineInterfaces"));
        return;
    }

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        FriendsInterface = OnlineSubsystem->GetFriendsInterface();
        UE_LOG(LogTemp, Warning, TEXT("FriendsInterface initialized: %s"),
            FriendsInterface.IsValid() ? TEXT("Success") : TEXT("Failed"));

        // 버튼 바인딩
        if (RefreshButton)
        {
            RefreshButton->OnClicked.RemoveAll(this); // 기존 바인딩 제거
            RefreshButton->OnClicked.AddDynamic(this, &USessionWidget::OnRefreshButtonClicked);
            UE_LOG(LogTemp, Warning, TEXT("RefreshButton bound successfully"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("RefreshButton is null"));
        }

        // 다른 초기화들...
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnlineSubsystem is null"));
    }
}

FFriendInfo USessionWidget::ConvertToFriendInfo(TSharedRef<FOnlineFriend> OnlineFriend)
{
    FFriendInfo FriendInfo;

    FriendInfo.DisplayName = OnlineFriend->GetDisplayName();

    // 안전한 UniqueNetId 변환
    TSharedPtr<const FUniqueNetId> UserId = OnlineFriend->GetUserId();
    if (UserId.IsValid())
    {
        FriendInfo.UniqueNetId = UserId->ToString();
    }
    else
    {
        FriendInfo.UniqueNetId = TEXT("Invalid");
        UE_LOG(LogTemp, Warning, TEXT("Invalid UserId for friend: %s"), *FriendInfo.DisplayName);
    }

    FriendInfo.bIsOnline = OnlineFriend->GetPresence().bIsOnline;
    FriendInfo.PresenceStatus = OnlineFriend->GetPresence().Status.StatusStr;
    FriendInfo.AvatarTexture = nullptr;

    return FriendInfo;
}
