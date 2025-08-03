// Fill out your copyright notice in the Description page of Project Settings.


#include "SRPlayerController.h"
#include "GameSetWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ASRPlayerController::ASRPlayerController()
{
    // �˻� ���[5][8][10]�� ���� �ùٸ� ���
    static ConstructorHelpers::FClassFinder<UGameSetWidget> GameSetWidgetFinder(TEXT("/Game/BP_Assets/UI/WBP_GameSetWidget.WBP_GameSetWidget_C"));
    if (GameSetWidgetFinder.Succeeded())
    {
        GameSetWidgetClass = GameSetWidgetFinder.Class;
        UE_LOG(LogTemp, Warning, TEXT("GameSetWidgetClass loaded successfully via FClassFinder"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load GameSetWidgetClass via FClassFinder"));
    }
}

void ASRPlayerController::BeginPlay()
{
	Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("PlayerController BeginPlay - GameSetWidgetClass: %s"),
        GameSetWidgetClass ? *GameSetWidgetClass->GetName() : TEXT("NULL"));
	SetInputByLevelName();
}

void ASRPlayerController::SetInputByLevelName()
{
	FString CurrentLevel = GetWorld()->GetMapName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (CurrentLevel == TEXT("ThirdPersonMap") || CurrentLevel == TEXT("Level_First") || CurrentLevel == TEXT("Level_Lobby") || CurrentLevel == TEXT("Level_First_New"))
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
		UE_LOG(LogTemp, Warning, TEXT("OpenLevel called! World name: %s"), *GetWorld()->GetName());
	}
}

void ASRPlayerController::Client_ShowGameResult_Implementation(const FString& ResultText)
{
    UE_LOG(LogTemp, Warning, TEXT("Client_ShowGameResult called on %s with result: %s"),
        *GetName(), *ResultText);

    // �̹� ������ �ִٸ� ����
    if (IsValid(GameSetWidget))
    {
        GameSetWidget->RemoveFromParent();
        GameSetWidget = nullptr;
    }

    // ���� Ŭ������ �����Ǿ� �ִ��� Ȯ��
    if (!GameSetWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("GameSetWidgetClass is not set in PlayerController"));
        return;
    }

    // Ŭ���̾�Ʈ���� ���� ����
    GameSetWidget = CreateWidget<UGameSetWidget>(this, GameSetWidgetClass);
    if (IsValid(GameSetWidget))
    {
        // ��� �ؽ�Ʈ ����
        if (IsValid(GameSetWidget->GameResultText))
        {
            GameSetWidget->GameResultText->SetText(FText::FromString(ResultText));
        }

        // ����Ʈ�� �߰�
        GameSetWidget->AddToViewport(999);
        GameSetWidget->SetVisibility(ESlateVisibility::Visible);

        // �Է� ��� ����
        SetShowMouseCursor(true);
        SetInputMode(FInputModeUIOnly());

        // ���� �Ͻ�����
        UGameplayStatics::SetGamePaused(GetWorld(), true);

        UE_LOG(LogTemp, Warning, TEXT("GameSet widget created and displayed successfully on %s"),
            *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create GameSet widget on %s"), *GetName());
    }
}
