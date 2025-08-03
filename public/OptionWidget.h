// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionWidget.generated.h"

class UComboBoxString;
class USlider;
class UButton;
class USoundMix;
class USoundClass;

UCLASS()
class SWORDRUSH_API UOptionWidget : public UUserWidget
{
    GENERATED_BODY()

    public:
    virtual void NativeConstruct() override;

    // 그래픽
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* FullscreenComboBox;
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* QualityComboBox;

    // 사운드
    UPROPERTY(meta = (BindWidget))
    USlider* MasterVolumeSlider;
    UPROPERTY(meta = (BindWidget))
    USlider* MusicVolumeSlider;
    UPROPERTY(meta = (BindWidget))
    USlider* SFXVolumeSlider;

    // 버튼
    UPROPERTY(meta = (BindWidget))
    UButton* ApplyButton;
    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;
protected:
    // 콜백
    UFUNCTION()
    void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnFullscreenChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnMusicVolumeChanged(float Value);

    UFUNCTION()
    void OnSFXVolumeChanged(float Value);

    UFUNCTION()
    void OnApplyClicked();

    UFUNCTION()
    void OnBackClicked();

    void SaveSettings();
    void LoadSettings();


    // 내부 상태
    FIntPoint PendingResolution;
    EWindowMode::Type PendingWindowMode;
    int32 PendingQualityLevel;
    float PendingMasterVolume;
    float PendingMusicVolume;
    float PendingSFXVolume;

    // 위젯 블루프린트에서 할당할 사운드 애셋 프로퍼티
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundMix* SettingsSoundMix;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundClass* MasterSoundClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundClass* MusicSoundClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundClass* SFXSoundClass;
};
