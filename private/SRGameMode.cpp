// Fill out your copyright notice in the Description page of Project Settings.


#include "SRGameMode.h"
#include "SRCharacter.h"
#include "SRPlayerController.h"
#include "SRPlayerState.h"

ASRGameMode::ASRGameMode()
{
    // 핵심 네트워크 설정만 C++에서
    PlayerStateClass = ASRPlayerState::StaticClass();
    PlayerControllerClass = ASRPlayerController::StaticClass();
    DefaultPawnClass = ASRCharacter::StaticClass();

    UE_LOG(LogTemp, Warning, TEXT("SRGameMode initialized with PlayerState"));
}

void ASRGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Blueprint 이벤트 호출
    OnGameStart();
}