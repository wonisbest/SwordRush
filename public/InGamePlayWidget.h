// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGamePlayWidget.generated.h"

/**
 * 
 */
class UButton;
class UTextBlock;
class UProgressBar;
class UBorder;
class UGameSetWidget;
class UWaitingPlayerWidget;
class UImage;
class ASRPlayerController;


UCLASS()
class SWORDRUSH_API UInGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MyHealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnemyHealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillCoolDownTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeTextBlock;

	UPROPERTY(meta = (BindWidget))
	UBorder* GameSetBorder;

	UPROPERTY(meta = (BindWidget))
	UImage* SkillIconImage;

	// Blueprint에서 WBP_GameSetWidget을 할당할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UGameSetWidget> GameSetWidgetClass;

	// Blueprint에서 WBP_GameSetWidget을 할당할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UWaitingPlayerWidget> WaitingPlayerWidgetClass;

	UPROPERTY()
	UWaitingPlayerWidget* WaitingPlayerWidget;

	UFUNCTION()
	void OnMyHealthChanged(float NewHealth);

	UFUNCTION()
	void OnEnemyHealthChanged(float NewHealth);

	UFUNCTION(Client, Reliable)
	void Client_ShowGameSet(const FString& ResultText);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;  // 추가

	UPROPERTY(Replicated)
	bool bGameEnded = false;

private:
	void UpdateHealthBar();
	void UpdateSkillCoolDown();
	void UpdateTimer();
	void OnTimerEnd();
	void GameSet();
	void GameSetTest();
	void CreateWaitingPlayerWidget();
	void SetSkillIcon();
	UFUNCTION(NetMulticast, Reliable)
	void GameStart();

	UPROPERTY()
	bool bMyHealthBound = false;
	// 캐릭터 찾기를 위한 함수
	void FindAndBindCharacters();

	class APlayerController* MyPC;
	class APlayerController* EnemyPC;

	class ASRCharacter* MyCharacter;
	class ASRCharacter* EnemyCharacter;

	// 타이머 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (AllowPrivateAccess = "true"))
	float GameTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (AllowPrivateAccess = "true"))
	float MaxGameTime;

	bool bIsGameActive;

	bool bIsGameReady = false;

	bool bSkillIconInitialized = false;

	UPROPERTY()
	bool bEnemyHealthBound = false;

};
