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

    // ģ�� ��� ������Ʈ
    UFUNCTION(BlueprintCallable, Category = "Session Widget")
    void UpdateFriendsList();

    // ģ�� ��� ���ΰ�ħ
    UFUNCTION(BlueprintCallable, Category = "Session Widget")
    void RefreshFriendsList();

    // ģ�� ����
    UFUNCTION(BlueprintCallable, Category = "Session Widget")
    void SortFriends();

    // ��������Ʈ
    UPROPERTY(BlueprintAssignable)
    FOnFriendsListUpdated OnFriendsListUpdatedEvent;

    FTimerHandle RefreshTimerHandle = FTimerHandle();
protected:
    // Widget ���ε�
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HeaderText;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* FriendsScrollBox;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* FriendsContainer;

    UPROPERTY(meta = (BindWidget))
    UButton* RefreshButton;

    // Social Widget Ŭ���� ���� (TSubclassOf ���)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Widget")
    TSubclassOf<class USocialWidget> SocialWidgetClass;

    // ģ�� ��� ������
    UPROPERTY(BlueprintReadOnly, Category = "Friends Data")
    TArray<FFriendInfo> FriendsList;

    // ������ Social Widget��
    UPROPERTY()
    TArray<USocialWidget*> SocialWidgets;

private:
    // OnlineSubsystem �������̽�
    TSharedPtr<class IOnlineFriends> FriendsInterface;

    // ģ�� ��� �б� �Ϸ� �ݹ�
    void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

    // Social Widget ����
    USocialWidget* CreateSocialWidget(const FFriendInfo& FriendInfo);

    // ��� Social Widget ����
    void ClearSocialWidgets();

    // ģ�� �ʴ� �̺�Ʈ ó��
    UFUNCTION()
    void OnFriendInvited(const FFriendInfo& FriendInfo);

    // ���ΰ�ħ ��ư Ŭ��
    UFUNCTION()
    void OnRefreshButtonClicked();
    void DelayedInitialize();
    void InitializeOnlineInterfaces();
    // Steam ģ�� �����͸� FFriendInfo�� ��ȯ
    FFriendInfo ConvertToFriendInfo(TSharedRef<FOnlineFriend> OnlineFriend);
};
