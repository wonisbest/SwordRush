// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MyGameInstance.h"
#include "SRCharacter.h"
#include "Components/ProgressBar.h"
#include "SKillCoolDownManager.h"
#include "Components/Border.h"
#include "GameSetWidget.h"
#include "Skill.h"
#include "Components/Image.h"
#include "WaitingPlayerWidget.h"
#include "GameFramework/HUD.h"
#include "EngineUtils.h"
#include "SRPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "SkillActorComponent.h"

void UInGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MaxGameTime = 183.0f;
	GameTime = MaxGameTime;
	bIsGameActive = true;
	bGameEnded = false;


	MyPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(MyPC))
	{
		MyCharacter = Cast<ASRCharacter>(MyPC->GetPawn());
		//MyCharacter->DisableInput(MyPC);
	}

	CreateWaitingPlayerWidget();
	UpdateTimer();

}

void UInGamePlayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bGameEnded) return;

	if (!bSkillIconInitialized && IsValid(MyCharacter) && IsValid(MyCharacter->SkillComponent->CurrentSkill))
	{
		bSkillIconInitialized = true;
		SetSkillIcon();
	}
	// 게임 준비 상태 확인
	if (!bIsGameReady)
	{
		int32 HumanPlayerCount = 0;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (Iterator->Get()->IsLocalPlayerController() || Iterator->Get()->GetPawn())
			{
				HumanPlayerCount++;
			}
		}

		if (HumanPlayerCount >= 2 || !MyPC->HasAuthority())
		{
			GameStart();
			bIsGameReady = true;
		}
		else
		{
			return;
		}
	}

	// 캐릭터 찾기 및 델리게이트 바인딩
	FindAndBindCharacters();

	if (bIsGameActive)
	{
		// 타이머 업데이트
		GameTime -= InDeltaTime;

		if (GameTime <= 0.0f)
		{
			GameTime = 0.0f;
			OnTimerEnd();
		}
		if (IsValid(MyCharacter) && IsValid(EnemyCharacter))
		{
			if (MyCharacter->Health <= 0.0f || EnemyCharacter->Health <= 0.0f)
			{
				GameSet();
			}
		}
		UpdateTimer();
		//UpdateHealthBar();
		UpdateSkillCoolDown();

	}
}

void UInGamePlayWidget::OnMyHealthChanged(float NewHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("OnMyHealthChanged called - NewHealth: %f for %s"),
		NewHealth, MyCharacter ? *MyCharacter->GetName() : TEXT("null"));

	if (IsValid(MyHealthBar))
	{
		float HealthPercent = NewHealth / 100.0f;
		MyHealthBar->SetPercent(HealthPercent);
		UE_LOG(LogTemp, Warning, TEXT("MyHealthBar updated to: %f%%"), HealthPercent * 100.0f);
	}
}

void UInGamePlayWidget::OnEnemyHealthChanged(float NewHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEnemyHealthChanged called - NewHealth: %f for %s"),
		NewHealth, EnemyCharacter ? *EnemyCharacter->GetName() : TEXT("null"));

	if (IsValid(EnemyHealthBar))
	{
		float HealthPercent = NewHealth / 100.0f;
		EnemyHealthBar->SetPercent(HealthPercent);
		UE_LOG(LogTemp, Warning, TEXT("EnemyHealthBar updated to: %f%%"), HealthPercent * 100.0f);
	}
}

void UInGamePlayWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInGamePlayWidget, bGameEnded);
}

void UInGamePlayWidget::Client_ShowGameSet_Implementation(const FString& ResultText)
{
	UE_LOG(LogTemp, Warning, TEXT("Client_ShowGameSet called with result: %s"), *ResultText);

	if (!GameSetWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GameSetWidgetClass is not set!"));
		return;
	}

	UGameSetWidget* GameSetWidget = CreateWidget<UGameSetWidget>(GetWorld(), GameSetWidgetClass);
	if (!IsValid(GameSetWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create GameSetWidget"));
		return;
	}

	// 결과 텍스트 설정
	if (IsValid(GameSetWidget->GameResultText))
	{
		GameSetWidget->GameResultText->SetText(FText::FromString(ResultText));
	}

	// UI 표시
	if (IsValid(GameSetBorder))
	{
		GameSetBorder->ClearChildren();
		GameSetBorder->AddChild(GameSetWidget);
		GameSetBorder->SetVisibility(ESlateVisibility::Visible);
	}

	GameSetWidget->SetVisibility(ESlateVisibility::Visible);

	// 입력 모드 변경
	if (IsValid(MyPC))
	{
		MyPC->SetShowMouseCursor(true);
		MyPC->SetInputMode(FInputModeUIOnly());
	}

	// 게임 일시정지
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UInGamePlayWidget::UpdateHealthBar()
{
	if (IsValid(MyCharacter))
	{
		float MyHealth = MyCharacter->Health / 100.0f;
		MyHealthBar->SetPercent(MyHealth);
	}
	
	if (IsValid(EnemyCharacter))
	{
		float EnemyHealth = EnemyCharacter->Health / 100.0f;
		EnemyHealthBar->SetPercent(EnemyHealth);
	}
	
	
	
}

void UInGamePlayWidget::UpdateSkillCoolDown()
{
	ASRCharacter* PlayerCharacter = Cast<ASRCharacter>(GetOwningPlayerPawn());
	if (!IsValid(PlayerCharacter) || !IsValid(PlayerCharacter->SkillComponent->SkillCoolDownManager))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter or SkillCoolDownManager is null"));
		return; // 이 한 줄로 충돌을 완벽하게 방지할 수 있습니다.
	}

	if (!IsValid(SkillCoolDownTextBlock))
	{
		UE_LOG(LogTemp, Error, TEXT("SkillCoolDownTextBlock is null"));
		return;
	}

	// 스킬 이름 안전하게 가져오기
	FName SkillName = TEXT("DefaultSkill");
	if (IsValid(PlayerCharacter->SkillComponent->CurrentSkill))
	{
		SkillName = PlayerCharacter->SkillComponent->CurrentSkill->SkillName;
	}

	//// 안전한 호출
	float RemainingTime = PlayerCharacter->SkillComponent->SkillCoolDownManager->GetRemainingCoolDown(SkillName);

	if (RemainingTime > 0.0f)
	{
		FString CoolDownText = FString::Printf(TEXT("%.0f"), RemainingTime);
		SkillCoolDownTextBlock->SetText(FText::FromString(CoolDownText));
	}
	else
	{
		SkillCoolDownTextBlock->SetText(FText::FromString(TEXT("")));
	}
	
}

void UInGamePlayWidget::UpdateTimer()
{
	if (IsValid(TimeTextBlock))
	{
		// 시간을 분:초 형식으로 변환
		int32 Minutes = FMath::FloorToInt(GameTime / 60.0f);
		int32 Seconds = FMath::FloorToInt(GameTime - (Minutes * 60.0f));

		FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TimeTextBlock->SetText(FText::FromString(TimeString));
	}
}

void UInGamePlayWidget::OnTimerEnd()
{
	bIsGameActive = false;

	// 게임 일시정지
	if (IsValid(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}

	// 타이머 종료 시 추가 로직 (게임 오버 화면 표시 등)
	if (IsValid(TimeTextBlock))
	{
		TimeTextBlock->SetText(FText::FromString(TEXT("TIME UP!")));
	}

	GameSet();
}

void UInGamePlayWidget::GameSet()
{
	UE_LOG(LogTemp, Warning, TEXT("=== GameSet() called - bGameEnded: %s ==="),
		bGameEnded ? TEXT("true") : TEXT("false"));

	// 이미 게임이 종료되었다면 리턴
	if (bGameEnded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game already ended, returning"));
		return;
	}
	bGameEnded = true;
	if (!IsValid(MyCharacter) || !IsValid(EnemyCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("Character validation failed"));
		return;
	}

	if (MyPC && MyPC->HasAuthority())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		// 게임 결과 상태 저장
		bool bPlayer1Won = false;
		bool bPlayer2Won = false;
		bool bDraw = false;

		// 전체 게임 결과 판정
		TArray<ASRCharacter*> AllCharacters;
		for (TActorIterator<ASRCharacter> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
		{
			AllCharacters.Add(*ActorIterator);
		}

		if (AllCharacters.Num() >= 2)
		{
			ASRCharacter* Char1 = AllCharacters[0];
			ASRCharacter* Char2 = AllCharacters[1];

			if (Char1->Health <= 0.0f)
			{
				bPlayer2Won = true;
			}
			else if (Char2->Health <= 0.0f)
			{
				bPlayer1Won = true;
			}
			else if (GameTime <= 0.0f)
			{
				if (Char1->Health > Char2->Health)
				{
					bPlayer1Won = true;
				}
				else if (Char2->Health > Char1->Health)
				{
					bPlayer2Won = true;
				}
				else
				{
					bDraw = true;
				}
			}
		}

		// 각 플레이어에게 개별 결과 전송
		int32 PlayerIndex = 0;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (ASRPlayerController* PC = Cast<ASRPlayerController>(Iterator->Get()))
			{
				FString ResultText;

				if (bDraw)
				{
					ResultText = TEXT("Draw");
				}
				else if ((PlayerIndex == 0 && bPlayer1Won) || (PlayerIndex == 1 && bPlayer2Won))
				{
					ResultText = TEXT("Win");
				}
				else
				{
					ResultText = TEXT("Lose");
				}

				UE_LOG(LogTemp, Warning, TEXT("Player %d gets result: %s"), PlayerIndex, *ResultText);
				PC->Client_ShowGameResult(ResultText);
				PlayerIndex++;
			}
		}
	}
}

void UInGamePlayWidget::GameSetTest()
{
	UE_LOG(LogTemp, Warning, TEXT("=== GameSetTest() called ==="));

	// GameSetWidgetClass가 Blueprint에서 설정되었는지 확인
	if (!GameSetWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GameSetWidgetClass is not set in Blueprint!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("GameSetWidgetClass is valid"));

	// GameSetWidget 생성
	UGameSetWidget* GameSetWidget = CreateWidget<UGameSetWidget>(GetWorld(), GameSetWidgetClass);
	if (!IsValid(GameSetWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create GameSetWidget"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("GameSetWidget created successfully"));

	// 테스트용 고정 결과
	FString ResultText = TEXT("Test Win");
	UE_LOG(LogTemp, Warning, TEXT("Test Result: %s"), *ResultText);

	// 결과 텍스트 설정
	if (IsValid(GameSetWidget->GameResultText))
	{
		GameSetWidget->GameResultText->SetText(FText::FromString(ResultText));
		UE_LOG(LogTemp, Warning, TEXT("Result text set"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameResultText is null"));
	}

	// GameSetBorder에 GameSetWidget 추가
	if (IsValid(GameSetBorder))
	{
		GameSetBorder->ClearChildren(); // 기존 자식 제거
		GameSetBorder->AddChild(GameSetWidget);
		UE_LOG(LogTemp, Warning, TEXT("GameSetWidget added to GameSetBorder"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameSetBorder is null!"));
		// 대안: 직접 뷰포트에 추가
		GameSetWidget->AddToViewport(999);
		UE_LOG(LogTemp, Warning, TEXT("GameSetWidget added to viewport as fallback"));
	}

	// 가시성 설정
	if (IsValid(GameSetBorder))
	{
		GameSetBorder->SetVisibility(ESlateVisibility::Visible);
	}
	GameSetWidget->SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogTemp, Warning, TEXT("Visibility set to Visible"));

	// 마우스 커서 표시
	if (IsValid(MyPC))
	{
		MyPC->SetShowMouseCursor(true);
		MyPC->SetInputMode(FInputModeUIOnly());
		UE_LOG(LogTemp, Warning, TEXT("Input mode set"));
	}

	// 게임 일시정지
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UE_LOG(LogTemp, Warning, TEXT("Game paused"));
}

void UInGamePlayWidget::CreateWaitingPlayerWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateWaitingPlayerWidget is called"));
	if (GetWorld()->GetNumPlayerControllers() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerNum is 1"));
		if (IsValid(GameSetBorder) && IsValid(WaitingPlayerWidgetClass))
		{
			WaitingPlayerWidget = CreateWidget<UWaitingPlayerWidget>(GetWorld(), WaitingPlayerWidgetClass);
			UE_LOG(LogTemp, Warning, TEXT("Create Widget"));
			if (IsValid(WaitingPlayerWidget))
			{
				if (IsValid(GameSetBorder))
				{
					GameSetBorder->ClearChildren(); // 기존 자식 제거
					GameSetBorder->AddChild(WaitingPlayerWidget);
					GameSetBorder->SetVisibility(ESlateVisibility::Visible);
					WaitingPlayerWidget->SetVisibility(ESlateVisibility::Visible);
					WaitingPlayerWidget->SetKeyboardFocus();
					UE_LOG(LogTemp, Warning, TEXT("Add Widget"));

					if (IsValid(MyPC))
					{
						MyCharacter->DisableInput(MyPC);
						UE_LOG(LogTemp, Warning, TEXT("SetInputMode FInputModeUIOnly"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("MyPC is Null"));
					}
					
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("GameSetBorder is null!"));
					// 대안: 직접 뷰포트에 추가
					WaitingPlayerWidget->AddToViewport(999);
					UE_LOG(LogTemp, Warning, TEXT("GameSetWidget added to viewport as fallback"));
				}
			}
		}
	}
}

void UInGamePlayWidget::SetSkillIcon()
{
	if (IsValid(SkillIconImage) && IsValid(MyCharacter))
	{
		if (!IsValid(MyCharacter->SkillComponent->CurrentSkill))
		{
			UE_LOG(LogTemp, Warning, TEXT("MyCharacter->SkillSet is not valid"));
			return;
		}
		if (IsValid(MyCharacter->SkillComponent->CurrentSkill->SKillIcon))
		{
			SkillIconImage->SetBrushFromTexture(MyCharacter->SkillComponent->CurrentSkill->SKillIcon);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MyCharacter->SkillSet->SKillIcon is not valid"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillIconImage and MyCharacter is not valid"));
	}
}

void UInGamePlayWidget::FindAndBindCharacters()
{
	// 내 캐릭터 바인딩
	if (!bMyHealthBound && IsValid(MyPC))
	{
		MyCharacter = Cast<ASRCharacter>(MyPC->GetPawn());
		if (IsValid(MyCharacter))
		{
			// 기존 바인딩 제거 후 재바인딩
			MyCharacter->OnHealthChanged.RemoveAll(this);
			MyCharacter->OnHealthChanged.AddDynamic(this, &UInGamePlayWidget::OnMyHealthChanged);

			// 초기 체력 설정
			OnMyHealthChanged(MyCharacter->Health);
			bMyHealthBound = true;

			UE_LOG(LogTemp, Warning, TEXT("MyCharacter (%s) health delegate bound - Initial Health: %f"),
				*MyCharacter->GetName(), MyCharacter->Health);
		}
	}

	// 적 캐릭터 바인딩 - 올바른 TActorIterator 사용법
	if (!bEnemyHealthBound && bIsGameReady)
	{
		// 검색 결과[5][6]에 따른 올바른 문법
		for (TActorIterator<ASRCharacter> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
		{
			ASRCharacter* Character = *ActorIterator;
			if (IsValid(Character) && Character != MyCharacter)
			{
				EnemyCharacter = Character;

				// 기존 바인딩 제거 후 재바인딩
				EnemyCharacter->OnHealthChanged.RemoveAll(this);
				EnemyCharacter->OnHealthChanged.AddDynamic(this, &UInGamePlayWidget::OnEnemyHealthChanged);

				// 초기 체력 설정
				OnEnemyHealthChanged(EnemyCharacter->Health);
				bEnemyHealthBound = true;

				UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter (%s) health delegate bound - Initial Health: %f"),
					*EnemyCharacter->GetName(), EnemyCharacter->Health);
				break;
			}
		}
	}
}

void UInGamePlayWidget::GameStart_Implementation()
{
	if (IsValid(WaitingPlayerWidget)) WaitingPlayerWidget->bIsGameReady = true;
}
