// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActorComponent.h"
#include "SRCharacter.h"
#include "SKillCoolDownManager.h"
#include "SkillList.h"
#include "Skill.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
// Sets default values for this component's properties
USkillActorComponent::USkillActorComponent()
{
    SetIsReplicatedByDefault(true);
}

void USkillActorComponent::RequestUseSkill()
{
    Server_UseSkill();
}


// Called when the game starts
void USkillActorComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ASRCharacter>(GetOwner());
    if (GetOwner()->HasAuthority())
    {
        SkillCoolDownManager = NewObject<USKillCoolDownManager>(this);
    }

    // 스킬리스트는 캐릭터 블루프린트에서 설정했다고 가정
    if (SkillsList && SkillsList->Skills.IsValidIndex(SkillsListIdx))
    {
        CurrentSkill = SkillsList->Skills[SkillsListIdx];
    }
	
}

void USkillActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USkillActorComponent, CurrentSkill);
    DOREPLIFETIME(USkillActorComponent, SkillCoolDownManager);
}

bool USkillActorComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
    bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

    // SkillCoolDownManager가 유효하다면, 이 객체 자체도 복제하도록 채널에 추가합니다.
    if (IsValid(SkillCoolDownManager))
    {
        bWroteSomething |= Channel->ReplicateSubobject(SkillCoolDownManager, *Bunch, *RepFlags);
    }

    return bWroteSomething;
}

void USkillActorComponent::Server_UseSkill_Implementation()
{
    if (!OwnerCharacter || !CurrentSkill || !SkillCoolDownManager) return;

    if (SkillCoolDownManager->IsOnCoolDown(CurrentSkill->SkillName))
    {
        UE_LOG(LogTemp, Log, TEXT("IsOnCoolDown : %f"), SkillCoolDownManager->GetRemainingCoolDown(CurrentSkill->SkillName));
        return;
    }

    SkillCoolDownManager->StartCoolDown(CurrentSkill->SkillName, CurrentSkill->CoolDownTime);
    CurrentSkill->ActivateSkill(OwnerCharacter);
    Multicast_PlaySkillMontage();
}

void USkillActorComponent::Multicast_PlaySkillMontage_Implementation()
{
    if (OwnerCharacter && CurrentSkill && CurrentSkill->SkillMontage)
    {
        OwnerCharacter->PlayAnimMontage(CurrentSkill->SkillMontage);
        OwnerCharacter->SetCharacterState(ECharacterState::Skill);
    }
}


