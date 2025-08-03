#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Engine/Texture2D.h"

#include "SocialWidget.generated.h"

// ģ�� ���� ����ü
USTRUCT(BlueprintType)
struct FFriendInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DisplayName;

    UPROPERTY(BlueprintReadOnly)
    FString UniqueNetId;

    UPROPERTY(BlueprintReadOnly)
    bool bIsOnline;

    UPROPERTY(BlueprintReadOnly)
    FString PresenceStatus;

    UPROPERTY(BlueprintReadOnly)
    UTexture2D* AvatarTexture;

    FFriendInfo()
    {
        DisplayName = "";
        UniqueNetId = "";
        bIsOnline = false;
        PresenceStatus = "";
        AvatarTexture = nullptr;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendInvited, const FFriendInfo&, FriendInfo);

UCLASS()
class SWORDRUSH_API USocialWidget : public UUserWidget
{
    GENERATED_BODY()

    public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ģ�� ���� ������Ʈ
    UFUNCTION(BlueprintCallable, Category = "Friend Widget")
    void UpdateFriendInfo(const FFriendInfo& InFriendInfo);

    // ģ�� �ʴ� ���
    UFUNCTION(BlueprintCallable, Category = "Friend Widget")
    void InviteFriend();

    // ��������Ʈ
    UPROPERTY(BlueprintAssignable)
    FOnFriendInvited OnFriendInvitedEvent;

    FTimerHandle RefreshTimerHandle = FTimerHandle();
public:
    // Widget ���ε� (Blueprint���� ������ ������� ����)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* PlayerNameText;

    UPROPERTY(BlueprintReadWrite ,meta = (BindWidget))
    UImage* AvatarImage;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* InviteButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UBorder* FriendBorder;

    // ģ�� ���� ����
    UPROPERTY(BlueprintReadOnly, Category = "Friend Data")
    FFriendInfo CurrentFriendInfo;

    // ��ư Ŭ�� �̺�Ʈ
    UFUNCTION()
    void OnInviteButtonClicked();

    // Steam �ƹ�Ÿ �ε� �Ϸ� �ݹ�
    UFUNCTION()
    void OnAvatarLoaded(UTexture2D* AvatarTexture);

private:
    // Steam ���� �������̽�
    TSharedPtr<class IOnlineSession> SessionInterface;
    TSharedPtr<class IOnlineFriends> FriendsInterface;

    // �ƹ�Ÿ �ε� ����
    void LoadSteamAvatar();
    void SetOnlineStatus(bool bIsOnline);
};
