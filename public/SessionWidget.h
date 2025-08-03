#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "SocialWidget.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Engine/TimerHandle.h"

#include "SessionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFriendsListUpdated, int32, OnlineFriendsCount, int32, TotalFriendsCount);

UCLASS()
class SWORDRUSH_API USessionWidget : public UUserWidget
{
    GENERATED_BODY()

    public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // 친구 목록 업데이트
    UFUNCTION(BlueprintCallable, Category = "Session Widget")
    void UpdateFriendsList();

    // 친구 목록 새로고침
    UFUNCTION(BlueprintCallable, Category = "Session Widget")
    void RefreshFriendsList();

    // 친구 정렬
    UFUNCTION(BlueprintCallable, Category = "Session Widget")
    void SortFriends();

    // 델리게이트
    UPROPERTY(BlueprintAssignable)
    FOnFriendsListUpdated OnFriendsListUpdatedEvent;

    FTimerHandle RefreshTimerHandle = FTimerHandle();
protected:
    // Widget 바인딩
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HeaderText;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* FriendsScrollBox;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* FriendsContainer;

    UPROPERTY(meta = (BindWidget))
    UButton* RefreshButton;

    // Social Widget 클래스 참조 (TSubclassOf 사용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Widget")
    TSubclassOf<class USocialWidget> SocialWidgetClass;

    // 친구 목록 데이터
    UPROPERTY(BlueprintReadOnly, Category = "Friends Data")
    TArray<FFriendInfo> FriendsList;

    // 생성된 Social Widget들
    UPROPERTY()
    TArray<USocialWidget*> SocialWidgets;

private:
    // OnlineSubsystem 인터페이스
    TSharedPtr<class IOnlineFriends> FriendsInterface;

    // 친구 목록 읽기 완료 콜백
    void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

    // Social Widget 생성
    USocialWidget* CreateSocialWidget(const FFriendInfo& FriendInfo);

    // 모든 Social Widget 제거
    void ClearSocialWidgets();

    // 친구 초대 이벤트 처리
    UFUNCTION()
    void OnFriendInvited(const FFriendInfo& FriendInfo);

    // 새로고침 버튼 클릭
    UFUNCTION()
    void OnRefreshButtonClicked();
    void DelayedInitialize();
    void InitializeOnlineInterfaces();
    // Steam 친구 데이터를 FFriendInfo로 변환
    FFriendInfo ConvertToFriendInfo(TSharedRef<FOnlineFriend> OnlineFriend);
};
