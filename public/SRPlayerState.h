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

	// ReplicatedUsing: 변수가 클라이언트에 복제될 때마다 OnRep_Stats 함수가 자동으로 호출됩니다.
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

	// 클라이언트가 서버에게 자신의 무기 선택을 알리는 RPC 함수입니다.
	UFUNCTION(Server, Reliable)
	void Server_SetWeaponIndex(int32 NewIndex);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
