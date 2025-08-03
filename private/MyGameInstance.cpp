#include "MyGameInstance.h"

void UMyGameInstance::Init()
{
    Super::Init();
    LoadPlayerData();

    UE_LOG(LogTemp, Log, TEXT("Game Instance Initialized - Ready for Blueprint Session Implementation"));
}


bool UMyGameInstance::SavePlayerData()
{
    UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    if (!SaveGameInstance) return false;

    SaveGameInstance->CustomizePartIndex = CurrentPartIndex;
    SaveGameInstance->StatValues = CurrentStats;
    SaveGameInstance->WeaponSetIndex = CurrentWeaponSet;
    SaveGameInstance->SkillSetIndex = CurrentSkillSet;
    const FString SlotName = TEXT("PlayerSaveSlot");
    const int32 UserIndex = 0;

    bool bSaveSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, UserIndex);

    UE_LOG(LogTemp, Log, TEXT("Save Player Data: %s"), bSaveSuccess ? TEXT("Success") : TEXT("Failed"));

    return bSaveSuccess;
}

bool UMyGameInstance::LoadPlayerData()
{
    if (!IsValid(this))
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance is invalid"));
        return false;
    }

    const FString SlotName = TEXT("PlayerSaveSlot");
    const int32 UserIndex = 0;

    if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
    {
        UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
        if (!LoadedGame) return false;
        
        CurrentPartIndex = LoadedGame->CustomizePartIndex;
        CurrentStats = LoadedGame->StatValues;
        CurrentWeaponSet = LoadedGame->WeaponSetIndex;
        CurrentSkillSet = LoadedGame->SkillSetIndex;

        IntailizeStats(CurrentStats);
        UE_LOG(LogTemp, Log, TEXT("Load Player Data: Success"));
        return true;
    }
    else
    {
        IntailizeStats(CurrentStats);
    }
    UE_LOG(LogTemp, Log, TEXT("Load Player Data: No save file found"));
    return false;
}

void UMyGameInstance::IntailizeStats(FSRPlayerStats& Stats)
{
    bool bIsCurrentStatsInitialized = (
        Stats.AttackPoint > 0 && Stats.AttackPoint <= 20 &&
        Stats.DeffensePoint > 0 && Stats.DeffensePoint <= 20 &&
        Stats.AttackSpeedPoint > 0 && Stats.AttackSpeedPoint <= 20 &&
        Stats.MoveSpeedPoint > 0 && Stats.MoveSpeedPoint <= 20
        );
    if (!bIsCurrentStatsInitialized)
    {
        ResetToDefaultStats();
    }
}

void UMyGameInstance::ResetToDefaultStats()
{
    CurrentStats.AttackPoint = 1;
    CurrentStats.DeffensePoint = 1;
    CurrentStats.AttackSpeedPoint = 1;
    CurrentStats.MoveSpeedPoint = 1;
    CurrentStats.StatPoint = 15;

    UE_LOG(LogTemp, Warning, TEXT("Stats reset to default values"));
    SavePlayerData();  // 기본값으로 저장
}
