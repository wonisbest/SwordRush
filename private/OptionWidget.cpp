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

    // �ػ� ��� ����
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

    // ��üȭ�� ��� ����
    if (FullscreenComboBox)
    {
        FullscreenComboBox->ClearOptions();
        FullscreenComboBox->AddOption(TEXT("Fullscreen"));
        FullscreenComboBox->AddOption(TEXT("WindowedFullscreen"));
        FullscreenComboBox->AddOption(TEXT("Windowed"));
        FullscreenComboBox->SetSelectedIndex(0);
        FullscreenComboBox->OnSelectionChanged.AddDynamic(this, &UOptionWidget::OnFullscreenChanged);
    }

    // ǰ�� ����
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

    // ���� �����̴� ����
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::OnMasterVolumeChanged);
        MasterVolumeSlider->SetValue(1.0f); // �ʱⰪ ����
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

    // ��ư
    if (ApplyButton)
        ApplyButton->OnClicked.AddDynamic(this, &UOptionWidget::OnApplyClicked);
    if (BackButton)
        BackButton->OnClicked.AddDynamic(this, &UOptionWidget::OnBackClicked);

    // �����̴� �ʱⰪ ����
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
    PendingMasterVolume = Value; // �� ������ �״�� �Ӵϴ�.
    if (SettingsSoundMix && MasterSoundClass)
    {
        // Ȱ��ȭ�� ���� �ͽ����� Ư�� ���� Ŭ������ ������ ��� �����մϴ�.
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
    // �׷���
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (Settings)
    {
        Settings->SetScreenResolution(PendingResolution);
        Settings->SetFullscreenMode(PendingWindowMode);
        Settings->SetOverallScalabilityLevel(PendingQualityLevel);
        Settings->ApplySettings(false);
    }
    // ����(�����̴����� �̹� �����)
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

    // ���� ���̺� ������ ������ �ҷ�����, ������ ���� �����մϴ�.
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
        // ���� ������ Pending ������ SaveGame ��ü�� ����ü�� �����մϴ�.
        SaveGameObject->OptionSettings.ScreenResolution = PendingResolution;
        SaveGameObject->OptionSettings.FullscreenMode = PendingWindowMode;
        SaveGameObject->OptionSettings.QualityLevel = PendingQualityLevel;
        SaveGameObject->OptionSettings.MasterVolume = PendingMasterVolume;
        SaveGameObject->OptionSettings.MusicVolume = PendingMusicVolume;
        SaveGameObject->OptionSettings.SFXVolume = PendingSFXVolume;

        // ���Ϸ� ����
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

    // ���̺� ������ �����ϸ� �� ����, ������ ����ü�� �⺻���� ����մϴ�.
    if (SaveGameObject)
    {
        PendingResolution = SaveGameObject->OptionSettings.ScreenResolution;
        PendingWindowMode = SaveGameObject->OptionSettings.FullscreenMode;
        PendingQualityLevel = SaveGameObject->OptionSettings.QualityLevel;
        PendingMasterVolume = SaveGameObject->OptionSettings.MasterVolume;
        PendingMusicVolume = SaveGameObject->OptionSettings.MusicVolume;
        PendingSFXVolume = SaveGameObject->OptionSettings.SFXVolume;
    }
    else // ���̺� ������ ������ �⺻�� ���
    {
        // FOptionSettings ����ü�� ���ǵ� �⺻������ �����ɴϴ�.
        FOptionSettings DefaultSettings;
        PendingResolution = DefaultSettings.ScreenResolution;
        PendingWindowMode = DefaultSettings.FullscreenMode;
        PendingQualityLevel = DefaultSettings.QualityLevel;
        PendingMasterVolume = DefaultSettings.MasterVolume;
        PendingMusicVolume = DefaultSettings.MusicVolume;
        PendingSFXVolume = DefaultSettings.SFXVolume;
    }

    // UI ��Ʈ��(�����̴�, �޺��ڽ�)�� �ҷ��� �� �Ǵ� �⺻���� �����մϴ�.
    if (MasterVolumeSlider) MasterVolumeSlider->SetValue(PendingMasterVolume);
    if (MusicVolumeSlider) MusicVolumeSlider->SetValue(PendingMusicVolume);
    if (SFXVolumeSlider) SFXVolumeSlider->SetValue(PendingSFXVolume);
    if (QualityComboBox) QualityComboBox->SetSelectedIndex(PendingQualityLevel);
    // (�ػ�, ��üȭ�� �޺��ڽ��� �ε����� ã�� �������ִ� ������ �ʿ��մϴ�)

    // �ҷ��� ������ ��� ���ӿ� �����մϴ�.
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
