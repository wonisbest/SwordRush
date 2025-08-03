#include "SocialWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Engine/Engine.h"
#include "Engine/Texture2D.h"
#include "OnlineSubsystem.h"
#include "AdvancedFriendsLibrary.h"

void USocialWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // OnlineSubsystem 인터페이스 초기화
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        SessionInterface = OnlineSubsystem->GetSessionInterface();
        FriendsInterface = OnlineSubsystem->GetFriendsInterface();
    }

    // 버튼 클릭 이벤트 바인딩
    if (InviteButton)
    {
        InviteButton->OnClicked.AddDynamic(this, &USocialWidget::OnInviteButtonClicked);
    }

    UE_LOG(LogTemp, Log, TEXT("FriendWidget: NativeConstruct completed"));
}

void USocialWidget::NativeDestruct()
{
    // 델리게이트 정리
    if (InviteButton)
    {
        InviteButton->OnClicked.RemoveAll(this);
    }

    Super::NativeDestruct();
}

void USocialWidget::UpdateFriendInfo(const FFriendInfo& InFriendInfo)
{
    CurrentFriendInfo = InFriendInfo;

    // 플레이어 이름 설정
    if (PlayerNameText)
    {
        PlayerNameText->SetText(FText::FromString(CurrentFriendInfo.DisplayName));
    }

    // 온라인 상태에 따른 투명도 설정
    SetOnlineStatus(CurrentFriendInfo.bIsOnline);

    // Steam 아바타 로드
    LoadSteamAvatar();

    UE_LOG(LogTemp, Log, TEXT("FriendWidget: Updated friend info for %s (Online: %s)"),
        *CurrentFriendInfo.DisplayName,
        CurrentFriendInfo.bIsOnline ? TEXT("Yes") : TEXT("No"));
}

void USocialWidget::InviteFriend()
{
    if (!SessionInterface.IsValid() || CurrentFriendInfo.UniqueNetId.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("FriendWidget: Cannot invite friend - invalid session or friend ID"));
        return;
    }

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("FriendWidget: OnlineSubsystem is not available"));
        return;
    }

    IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
    if (!IdentityInterface.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("FriendWidget: IdentityInterface is not valid"));
        return;
    }

    // Steam 플랫폼에서 UniqueNetId 생성
    TSharedPtr<const FUniqueNetId> FriendNetId = IdentityInterface->CreateUniquePlayerId(CurrentFriendInfo.UniqueNetId);
    if (!FriendNetId.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("FriendWidget: Failed to create UniqueNetId for %s"), *CurrentFriendInfo.DisplayName);
        return;
    }

    // 세션 초대 전송
    bool bInviteSuccess = SessionInterface->SendSessionInviteToFriend(0, NAME_GameSession, *FriendNetId);

    if (bInviteSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("FriendWidget: Sent invite to %s"), *CurrentFriendInfo.DisplayName);

        // 델리게이트 브로드캐스트
        OnFriendInvitedEvent.Broadcast(CurrentFriendInfo);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
                FString::Printf(TEXT("Invited %s to game!"), *CurrentFriendInfo.DisplayName));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FriendWidget: Failed to send invite to %s"), *CurrentFriendInfo.DisplayName);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
                FString::Printf(TEXT("Failed to invite %s"), *CurrentFriendInfo.DisplayName));
        }
    }

}

void USocialWidget::OnInviteButtonClicked()
{
    //UE_LOG(LogTemp, Log, TEXT("FriendWidget: Invite button clicked for %s"), *CurrentFriendInfo.DisplayName);
    //InviteFriend();
}

void USocialWidget::LoadSteamAvatar()
{
    // Steam 아바타 로딩 로직
    // 실제 구현에서는 Steam API를 통해 아바타 이미지를 비동기로 로드해야 함
    if (IsValid(AvatarImage) && IsValid(CurrentFriendInfo.AvatarTexture))
    {
        AvatarImage->SetBrushFromTexture(CurrentFriendInfo.AvatarTexture);
    }
    else if (AvatarImage)
    {
        // 기본 아바타 이미지 설정
        // UTexture2D* DefaultAvatar = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Textures/DefaultAvatar"));
        // AvatarImage->SetBrushFromTexture(DefaultAvatar);
    }
}

void USocialWidget::SetOnlineStatus(bool bIsOnline)
{
    if (FriendBorder)
    {
        // 온라인 상태에 따른 투명도 조절
        float Opacity = bIsOnline ? 1.0f : 0.5f;
        FriendBorder->SetRenderOpacity(Opacity);
    }

    // 버튼 활성화/비활성화
    if (InviteButton)
    {
        InviteButton->SetIsEnabled(bIsOnline);
    }
}

void USocialWidget::OnAvatarLoaded(UTexture2D* AvatarTexture)
{
    if (AvatarTexture && AvatarImage)
    {
        CurrentFriendInfo.AvatarTexture = AvatarTexture;
        AvatarImage->SetBrushFromTexture(AvatarTexture);

        UE_LOG(LogTemp, Log, TEXT("FriendWidget: Avatar loaded for %s"), *CurrentFriendInfo.DisplayName);
    }
}
