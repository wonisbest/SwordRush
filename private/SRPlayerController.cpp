// Fill out your copyright notice in the Description page of Project Settings.


#include "SRPlayerController.h"
#include "GameSetWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ASRPlayerController::ASRPlayerController()
{
    // 검색 결과[5][8][10]에 따른 올바른 경로
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

    // 이미 위젯이 있다면 제거
    if (IsValid(GameSetWidget))
    {
        GameSetWidget->RemoveFromParent();
        GameSetWidget = nullptr;
    }

    // 위젯 클래스가 설정되어 있는지 확인
    if (!GameSetWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("GameSetWidgetClass is not set in PlayerController"));
        return;
    }

    // 클라이언트에서 위젯 생성
    GameSetWidget = CreateWidget<UGameSetWidget>(this, GameSetWidgetClass);
    if (IsValid(GameSetWidget))
    {
        // 결과 텍스트 설정
        if (IsValid(GameSetWidget->GameResultText))
        {
            GameSetWidget->GameResultText->SetText(FText::FromString(ResultText));
        }

        // 뷰포트에 추가
        GameSetWidget->AddToViewport(999);
        GameSetWidget->SetVisibility(ESlateVisibility::Visible);

        // 입력 모드 변경
        SetShowMouseCursor(true);
        SetInputMode(FInputModeUIOnly());

        // 게임 일시정지
        UGameplayStatics::SetGamePaused(GetWorld(), true);

        UE_LOG(LogTemp, Warning, TEXT("GameSet widget created and displayed successfully on %s"),
            *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create GameSet widget on %s"), *GetName());
    }
}
