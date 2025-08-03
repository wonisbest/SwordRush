#include "OptionWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "GameFramework/GameUserSettings.h"
#include "MySaveGame.h"

void UOptionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 해상도 목록 세팅
    if (ResolutionComboBox)
    {
        ResolutionComboBox->ClearOptions();
        ResolutionComboBox->AddOption(TEXT("3840x2160"));
        ResolutionComboBox->AddOption(TEXT("2560x1440"));
        ResolutionComboBox->AddOption(TEXT("1920x1080"));
        ResolutionComboBox->AddOption(TEXT("1600x900"));
        ResolutionComboBox->AddOption(TEXT("1280x720"));
        ResolutionComboBox->SetSelectedIndex(2);
        ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UOptionWidget::OnResolutionChanged);
    }

    // 전체화면 모드 세팅
    if (FullscreenComboBox)
    {
        FullscreenComboBox->ClearOptions();
        FullscreenComboBox->AddOption(TEXT("Fullscreen"));
        FullscreenComboBox->AddOption(TEXT("WindowedFullscreen"));
        FullscreenComboBox->AddOption(TEXT("Windowed"));
        FullscreenComboBox->SetSelectedIndex(0);
        FullscreenComboBox->OnSelectionChanged.AddDynamic(this, &UOptionWidget::OnFullscreenChanged);
    }

    // 품질 세팅
    if (QualityComboBox)
    {
        QualityComboBox->ClearOptions();
        QualityComboBox->AddOption(TEXT("Low"));
        QualityComboBox->AddOption(TEXT("Medium"));
        QualityComboBox->AddOption(TEXT("High"));
        QualityComboBox->AddOption(TEXT("Epic"));
        QualityComboBox->SetSelectedIndex(3);
        QualityComboBox->OnSelectionChanged.AddDynamic(this, &UOptionWidget::OnQualityChanged);
    }

    if (SettingsSoundMix)
    {
        UGameplayStatics::PushSoundMixModifier(this, SettingsSoundMix);
    }

    // 사운드 슬라이더 세팅
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnMasterVolumeChanged);
        MasterVolumeSlider->SetValue(1.0f); // 초기값 설정
    }
    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnMusicVolumeChanged);
        MusicVolumeSlider->SetValue(1.0f);
    }
    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnSFXVolumeChanged);
        SFXVolumeSlider->SetValue(1.0f);
    }

    // 버튼
    if (ApplyButton)
        ApplyButton->OnClicked.AddDynamic(this, &UOptionWidget::OnApplyClicked);
    if (BackButton)
        BackButton->OnClicked.AddDynamic(this, &UOptionWidget::OnBackClicked);

    // 슬라이더 초기값 세팅
    if (MasterVolumeSlider) MasterVolumeSlider->SetValue(PendingMasterVolume);
    if (MusicVolumeSlider) MusicVolumeSlider->SetValue(PendingMusicVolume);
    if (SFXVolumeSlider) SFXVolumeSlider->SetValue(PendingSFXVolume);

    LoadSettings();
}

void UOptionWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type)
{
    if (SelectedItem == "3840x2160") PendingResolution = FIntPoint(3840, 2160);
    else if (SelectedItem == "2560x1440") PendingResolution = FIntPoint(2560, 1440);
    else if (SelectedItem == "1920x1080") PendingResolution = FIntPoint(1920, 1080);
    else if (SelectedItem == "1600x900") PendingResolution = FIntPoint(1600, 900);
    else if (SelectedItem == "1280x720") PendingResolution = FIntPoint(1280, 720);
}

void UOptionWidget::OnFullscreenChanged(FString SelectedItem, ESelectInfo::Type)
{
    if (SelectedItem == "Fullscreen") PendingWindowMode = EWindowMode::Fullscreen;
    else if (SelectedItem == "WindowedFullscreen") PendingWindowMode = EWindowMode::WindowedFullscreen;
    else if (SelectedItem == "Windowed") PendingWindowMode = EWindowMode::Windowed;
}

void UOptionWidget::OnQualityChanged(FString SelectedItem, ESelectInfo::Type)
{
    if (SelectedItem == "Low") PendingQualityLevel = 0;
    else if (SelectedItem == "Medium") PendingQualityLevel = 1;
    else if (SelectedItem == "High") PendingQualityLevel = 2;
    else if (SelectedItem == "Epic") PendingQualityLevel = 3;
}

void UOptionWidget::OnMasterVolumeChanged(float Value)
{
    PendingMasterVolume = Value; // 값 저장은 그대로 둡니다.
    if (SettingsSoundMix && MasterSoundClass)
    {
        // 활성화된 사운드 믹스에서 특정 사운드 클래스의 볼륨을 즉시 변경합니다.
        UGameplayStatics::SetSoundMixClassOverride(this, SettingsSoundMix, MasterSoundClass, Value, 1.0f, 0.0f, true);
    }
}

void UOptionWidget::OnMusicVolumeChanged(float Value)
{
    PendingMusicVolume = Value;
    if (SettingsSoundMix && MusicSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(this, SettingsSoundMix, MusicSoundClass, Value, 1.0f, 0.0f, true);
    }
}

void UOptionWidget::OnSFXVolumeChanged(float Value)
{
    PendingSFXVolume = Value;
    if (SettingsSoundMix && SFXSoundClass)
    {
        UGameplayStatics::SetSoundMixClassOverride(this, SettingsSoundMix, SFXSoundClass, Value, 1.0f, 0.0f, true);
    }
}

void UOptionWidget::OnApplyClicked()
{
    // 그래픽
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (Settings)
    {
        Settings->SetScreenResolution(PendingResolution);
        Settings->SetFullscreenMode(PendingWindowMode);
        Settings->SetOverallScalabilityLevel(PendingQualityLevel);
        Settings->ApplySettings(false);
    }
    // 사운드(슬라이더에서 이미 적용됨)
    SaveSettings();
}

void UOptionWidget::OnBackClicked()
{
    RemoveFromParent();
    UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
    NewWidget->AddToViewport();
}

void UOptionWidget::SaveSettings()
{
    const FString SaveSlotName = TEXT("GameSettingsSlot");

    // 기존 세이브 파일이 있으면 불러오고, 없으면 새로 생성합니다.
    UMySaveGame* SaveGameObject = nullptr;
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        SaveGameObject = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    }
    else
    {
        SaveGameObject = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    }

    if (SaveGameObject)
    {
        // 현재 위젯의 Pending 값들을 SaveGame 객체의 구조체에 저장합니다.
        SaveGameObject->OptionSettings.ScreenResolution = PendingResolution;
        SaveGameObject->OptionSettings.FullscreenMode = PendingWindowMode;
        SaveGameObject->OptionSettings.QualityLevel = PendingQualityLevel;
        SaveGameObject->OptionSettings.MasterVolume = PendingMasterVolume;
        SaveGameObject->OptionSettings.MusicVolume = PendingMusicVolume;
        SaveGameObject->OptionSettings.SFXVolume = PendingSFXVolume;

        // 파일로 저장
        UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);
    }
}

void UOptionWidget::LoadSettings()
{
    const FString SaveSlotName = TEXT("GameSettingsSlot");
    UMySaveGame* SaveGameObject = nullptr;

    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        SaveGameObject = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    }

    // 세이브 파일이 존재하면 그 값을, 없으면 구조체의 기본값을 사용합니다.
    if (SaveGameObject)
    {
        PendingResolution = SaveGameObject->OptionSettings.ScreenResolution;
        PendingWindowMode = SaveGameObject->OptionSettings.FullscreenMode;
        PendingQualityLevel = SaveGameObject->OptionSettings.QualityLevel;
        PendingMasterVolume = SaveGameObject->OptionSettings.MasterVolume;
        PendingMusicVolume = SaveGameObject->OptionSettings.MusicVolume;
        PendingSFXVolume = SaveGameObject->OptionSettings.SFXVolume;
    }
    else // 세이브 파일이 없으면 기본값 사용
    {
        // FOptionSettings 구조체에 정의된 기본값들을 가져옵니다.
        FOptionSettings DefaultSettings;
        PendingResolution = DefaultSettings.ScreenResolution;
        PendingWindowMode = DefaultSettings.FullscreenMode;
        PendingQualityLevel = DefaultSettings.QualityLevel;
        PendingMasterVolume = DefaultSettings.MasterVolume;
        PendingMusicVolume = DefaultSettings.MusicVolume;
        PendingSFXVolume = DefaultSettings.SFXVolume;
    }

    // UI 컨트롤(슬라이더, 콤보박스)에 불러온 값 또는 기본값을 적용합니다.
    if (MasterVolumeSlider) MasterVolumeSlider->SetValue(PendingMasterVolume);
    if (MusicVolumeSlider) MusicVolumeSlider->SetValue(PendingMusicVolume);
    if (SFXVolumeSlider) SFXVolumeSlider->SetValue(PendingSFXVolume);
    if (QualityComboBox) QualityComboBox->SetSelectedIndex(PendingQualityLevel);
    // (해상도, 전체화면 콤보박스도 인덱스를 찾아 설정해주는 로직이 필요합니다)

    // 불러온 설정을 즉시 게임에 적용합니다.
    UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();
    if (GameSettings)
    {
        GameSettings->SetScreenResolution(PendingResolution);
        GameSettings->SetFullscreenMode(PendingWindowMode);
        GameSettings->SetOverallScalabilityLevel(PendingQualityLevel);
        GameSettings->ApplySettings(false);
    }
    OnMasterVolumeChanged(PendingMasterVolume);
    OnMusicVolumeChanged(PendingMusicVolume);
    OnSFXVolumeChanged(PendingSFXVolume);
}
