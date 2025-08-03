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

    // Blueprint���� �������̵� ������ �̺�Ʈ��
    UFUNCTION(BlueprintImplementableEvent)
    void OnGameStart();

    UFUNCTION(BlueprintImplementableEvent)
    void OnPlayerJoined(APlayerController* NewPlayer);
};
