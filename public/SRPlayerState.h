// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MySaveGame.h"
#include "SRPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SWORDRUSH_API ASRPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASRPlayerState();

	// ReplicatedUsing: ������ Ŭ���̾�Ʈ�� ������ ������ OnRep_Stats �Լ��� �ڵ����� ȣ��˴ϴ�.
	UPROPERTY(ReplicatedUsing = OnRep_Stats)
	FSRPlayerStats CurrentStats;

	UFUNCTION()
	void OnRep_Stats();

	UFUNCTION(Server, Reliable)
	void Server_SetStats(const FSRPlayerStats& NewStats);

	UPROPERTY(ReplicatedUsing = OnRep_WeaponIndex)
	int32 WeaponIndex;

	UFUNCTION()
	void OnRep_WeaponIndex();

	// Ŭ���̾�Ʈ�� �������� �ڽ��� ���� ������ �˸��� RPC �Լ��Դϴ�.
	UFUNCTION(Server, Reliable)
	void Server_SetWeaponIndex(int32 NewIndex);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
