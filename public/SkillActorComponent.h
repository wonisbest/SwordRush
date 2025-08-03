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

	// ��ų ����� ��û�ϴ� �Լ� (ĳ���Ͱ� ȣ��)
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

	// �������� ���� ��ų�� ����ϴ� �Լ�
	UFUNCTION(Server, Reliable)
	void Server_UseSkill();

	// ��� Ŭ���̾�Ʈ���� ��ų �ִϸ��̼��� �����Ű�� �Լ�
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySkillMontage();
};
