// Fill out your copyright notice in the Description page of Project Settings.


#include "SRPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SRCharacter.h"

ASRPlayerState::ASRPlayerState()
{
    // 기본값 초기화
    CurrentStats.AttackPoint = 1;
    CurrentStats.DeffensePoint = 1;
    CurrentStats.AttackSpeedPoint = 1;
    CurrentStats.MoveSpeedPoint = 1;
    CurrentStats.StatPoint = 15;
}

void ASRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASRPlayerState, CurrentStats); // CurrentStats 구조체 전체를 복제
}

void ASRPlayerState::OnRep_Stats()
{
    // 스탯이 변경되었음을 캐릭터에게 알림 (UI 업데이트 등에 사용)
    if (ASRCharacter* MyCharacter = Cast<ASRCharacter>(GetPawn()))
    {
        MyCharacter->OnPlayerStatChanged();
    }
}

void ASRPlayerState::Server_SetStats_Implementation(const FSRPlayerStats& NewStats)
{
    CurrentStats = NewStats;
    // RepNotify는 서버에서 직접 호출되지 않으므로, 서버 캐릭터에도 변경사항을 알리기 위해 수동 호출
    OnRep_Stats();
}

void ASRPlayerState::OnRep_WeaponIndex()
{
    ASRCharacter* MyCharacter = Cast<ASRCharacter>(GetPawn());
    if (MyCharacter)
    {
        // 서버가 설정한 무기 인덱스를 클라이언트 캐릭터에 반영하고, 그에 맞는 무기를 생성하도록 요청합니다.
        MyCharacter->WeaponIdx = WeaponIndex;
        MyCharacter->SetWeaponMesh();
    }
}

void ASRPlayerState::Server_SetWeaponIndex_Implementation(int32 NewIndex)
{
    // 클라이언트가 보내온 인덱스로 서버의 값을 갱신합니다.
    WeaponIndex = NewIndex;

    // 서버 자신의 캐릭터에도 즉시 적용합니다.
    OnRep_WeaponIndex();
}