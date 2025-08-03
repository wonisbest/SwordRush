// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SRGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SWORDRUSH_API ASRGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    ASRGameMode();

protected:
    virtual void BeginPlay() override;

    // Blueprint에서 오버라이드 가능한 이벤트들
    UFUNCTION(BlueprintImplementableEvent)
    void OnGameStart();

    UFUNCTION(BlueprintImplementableEvent)
    void OnPlayerJoined(APlayerController* NewPlayer);
};
