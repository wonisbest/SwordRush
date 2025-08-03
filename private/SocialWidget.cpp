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

    // OnlineSubsystem �������̽� �ʱ�ȭ
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        SessionInterface = OnlineSubsystem->GetSessionInterface();
        FriendsInterface = OnlineSubsystem->GetFriendsInterface();
    }

    // ��ư Ŭ�� �̺�Ʈ ���ε�
    if (InviteButton)
    {
        InviteButton->OnClicked.AddDynamic(this, &USocialWidget::OnInviteButtonClicked);
    }

    UE_LOG(LogTemp, Log, TEXT("FriendWidget: NativeConstruct completed"));
}

void USocialWidget::NativeDestruct()
{
    // ��������Ʈ ����
    if (InviteButton)
    {
        InviteButton->OnClicked.RemoveAll(this);
    }

    Super::NativeDestruct();
}

void USocialWidget::UpdateFriendInfo(const FFriendInfo& InFriendInfo)
{
    CurrentFriendInfo = InFriendInfo;

    // �÷��̾� �̸� ����
    if (PlayerNameText)
    {
        PlayerNameText->SetText(FText::FromString(CurrentFriendInfo.DisplayName));
    }

    // �¶��� ���¿� ���� ���� ����
    SetOnlineStatus(CurrentFriendInfo.bIsOnline);

    // Steam �ƹ�Ÿ �ε�
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

    // Steam �÷������� UniqueNetId ����
    TSharedPtr<const FUniqueNetId> FriendNetId = IdentityInterface->CreateUniquePlayerId(CurrentFriendInfo.UniqueNetId);
    if (!FriendNetId.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("FriendWidget: Failed to create UniqueNetId for %s"), *CurrentFriendInfo.DisplayName);
        return;
    }

    // ���� �ʴ� ����
    bool bInviteSuccess = SessionInterface->SendSessionInviteToFriend(0, NAME_GameSession, *FriendNetId);

    if (bInviteSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("FriendWidget: Sent invite to %s"), *CurrentFriendInfo.DisplayName);

        // ��������Ʈ ��ε�ĳ��Ʈ
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
    // Steam �ƹ�Ÿ �ε� ����
    // ���� ���������� Steam API�� ���� �ƹ�Ÿ �̹����� �񵿱�� �ε��ؾ� ��
    if (IsValid(AvatarImage) && IsValid(CurrentFriendInfo.AvatarTexture))
    {
        AvatarImage->SetBrushFromTexture(CurrentFriendInfo.AvatarTexture);
    }
    else if (AvatarImage)
    {
        // �⺻ �ƹ�Ÿ �̹��� ����
        // UTexture2D* DefaultAvatar = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Textures/DefaultAvatar"));
        // AvatarImage->SetBrushFromTexture(DefaultAvatar);
    }
}

void USocialWidget::SetOnlineStatus(bool bIsOnline)
{
    if (FriendBorder)
    {
        // �¶��� ���¿� ���� ���� ����
        float Opacity = bIsOnline ? 1.0f : 0.5f;
        FriendBorder->SetRenderOpacity(Opacity);
    }

    // ��ư Ȱ��ȭ/��Ȱ��ȭ
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
