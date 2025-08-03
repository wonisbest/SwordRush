// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillActorComponent.generated.h"

class USkillList;
class USkill;
class USKillCoolDownManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDRUSH_API USkillActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillActorComponent();

	// 스킬 사용을 요청하는 함수 (캐릭터가 호출)
	void RequestUseSkill();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
public:

	UPROPERTY(EditAnywhere, Category = "Skill")
	TObjectPtr<class ASRCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TObjectPtr<USkillList> SkillsList;

	UPROPERTY()
	int32 SkillsListIdx = 0;

	UPROPERTY(Replicated)
	TObjectPtr<USkill> CurrentSkill;

	UPROPERTY(Replicated)
	TObjectPtr<USKillCoolDownManager> SkillCoolDownManager;

	// 서버에서 실제 스킬을 사용하는 함수
	UFUNCTION(Server, Reliable)
	void Server_UseSkill();

	// 모든 클라이언트에게 스킬 애니메이션을 재생시키는 함수
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySkillMontage();
};
