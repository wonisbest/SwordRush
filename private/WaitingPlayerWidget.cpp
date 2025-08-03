// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingPlayerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "SRCharacter.h"
void UWaitingPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CountDownTime = 3.0f;
	bIsGameReady = false;

	bIsFocusable = true;
	
}

void UWaitingPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsGameReady)
	{
		CountDownTime -= InDeltaTime;
		CountDown();
	}
	else
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PC->SetInputMode(FInputModeUIOnly());
	}
	if (CountDownTime <= 0.5f)
	{
		bIsGameReady = false;
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ASRCharacter* MyChar = Cast<ASRCharacter>(PC->GetPawn());
		PC->SetInputMode(FInputModeGameOnly());
		MyChar->EnableInput(PC);
		bIsFocusable = false;
		RemoveFromParent();
	}
}

void UWaitingPlayerWidget::CountDown()
{
	if (IsValid(WaitingAnnouncementTextBlock))
	{
		// 시간을 초 형식으로 변환
		int32 Seconds = FMath::FloorToInt(CountDownTime );

		FString TimeString = FString::Printf(TEXT("%d"), Seconds);
		WaitingAnnouncementTextBlock->SetText(FText::FromString(TimeString));
	}
}

FReply UWaitingPlayerWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		// ESC 처리 (예: 메뉴 닫기)
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		ASRCharacter* MyChar = Cast<ASRCharacter>(PC->GetPawn());
		MyChar->GamePause();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}
