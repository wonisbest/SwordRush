// Fill out your copyright notice in the Description page of Project Settings.


#include "SRPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SRCharacter.h"

ASRPlayerState::ASRPlayerState()
{
    // �⺻�� �ʱ�ȭ
    CurrentStats.AttackPoint = 1;
    CurrentStats.DeffensePoint = 1;
    CurrentStats.AttackSpeedPoint = 1;
    CurrentStats.MoveSpeedPoint = 1;
    CurrentStats.StatPoint = 15;
}

void ASRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASRPlayerState, CurrentStats); // CurrentStats ����ü ��ü�� ����
}

void ASRPlayerState::OnRep_Stats()
{
    // ������ ����Ǿ����� ĳ���Ϳ��� �˸� (UI ������Ʈ � ���)
    if (ASRCharacter* MyCharacter = Cast<ASRCharacter>(GetPawn()))
    {
        MyCharacter->OnPlayerStatChanged();
    }
}

void ASRPlayerState::Server_SetStats_Implementation(const FSRPlayerStats& NewStats)
{
    CurrentStats = NewStats;
    // RepNotify�� �������� ���� ȣ����� �����Ƿ�, ���� ĳ���Ϳ��� ��������� �˸��� ���� ���� ȣ��
    OnRep_Stats();
}

void ASRPlayerState::OnRep_WeaponIndex()
{
    ASRCharacter* MyCharacter = Cast<ASRCharacter>(GetPawn());
    if (MyCharacter)
    {
        // ������ ������ ���� �ε����� Ŭ���̾�Ʈ ĳ���Ϳ� �ݿ��ϰ�, �׿� �´� ���⸦ �����ϵ��� ��û�մϴ�.
        MyCharacter->WeaponIdx = WeaponIndex;
        MyCharacter->SetWeaponMesh();
    }
}

void ASRPlayerState::Server_SetWeaponIndex_Implementation(int32 NewIndex)
{
    // Ŭ���̾�Ʈ�� ������ �ε����� ������ ���� �����մϴ�.
    WeaponIndex = NewIndex;

    // ���� �ڽ��� ĳ���Ϳ��� ��� �����մϴ�.
    OnRep_WeaponIndex();
}