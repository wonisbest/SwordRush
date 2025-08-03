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

// 친구 정보 구조체
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

    // 친구 정보 업데이트
    UFUNCTION(BlueprintCallable, Category = "Friend Widget")
    void UpdateFriendInfo(const FFriendInfo& InFriendInfo);

    // 친구 초대 기능
    UFUNCTION(BlueprintCallable, Category = "Friend Widget")
    void InviteFriend();

    // 델리게이트
    UPROPERTY(BlueprintAssignable)
    FOnFriendInvited OnFriendInvitedEvent;

    FTimerHandle RefreshTimerHandle = FTimerHandle();
public:
    // Widget 바인딩 (Blueprint에서 생성된 위젯들과 연결)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* PlayerNameText;

    UPROPERTY(BlueprintReadWrite ,meta = (BindWidget))
    UImage* AvatarImage;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* InviteButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UBorder* FriendBorder;

    // 친구 정보 저장
    UPROPERTY(BlueprintReadOnly, Category = "Friend Data")
    FFriendInfo CurrentFriendInfo;

    // 버튼 클릭 이벤트
    UFUNCTION()
    void OnInviteButtonClicked();

    // Steam 아바타 로드 완료 콜백
    UFUNCTION()
    void OnAvatarLoaded(UTexture2D* AvatarTexture);

private:
    // Steam 관련 인터페이스
    TSharedPtr<class IOnlineSession> SessionInterface;
    TSharedPtr<class IOnlineFriends> FriendsInterface;

    // 아바타 로딩 관련
    void LoadSteamAvatar();
    void SetOnlineStatus(bool bIsOnline);
};
