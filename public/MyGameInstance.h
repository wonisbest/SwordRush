#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
//#include "Engine/GameInstance.h"
#include "MySaveGame.h"
#include "Kismet/GamePlayStatics.h"
//#include "AdvancedFriendsGameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SWORDRUSH_API UMyGameInstance : public UAdvancedFriendsGameInstance
{
    GENERATED_BODY()

    public:
    virtual void Init() override;


    // �÷��̾� ������ ���� (����)
    UPROPERTY(BlueprintReadWrite, Category = "Player Data")
    FPartIndex CurrentPartIndex;

    UPROPERTY(BlueprintReadWrite, Category = "Player Data")
    FSRPlayerStats CurrentStats;

    UPROPERTY(BlueprintReadWrite, Category = "Player Data")
    FWeaponSet CurrentWeaponSet;

    UPROPERTY(BlueprintReadWrite, Category = "Player Data")
    FSkillSet CurrentSkillSet;

    // Save/Load ��� (����)
    UFUNCTION(BlueprintCallable, Category = "Save System")
    bool SavePlayerData();

    UFUNCTION(BlueprintCallable, Category = "Save System")
    bool LoadPlayerData();

    void IntailizeStats(FSRPlayerStats& Stats);

    UFUNCTION(BlueprintCallable)
    void ResetToDefaultStats();
};
