// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyParryEnd.h"
#include "SRCharacter.h"
#include "CombatActorComponent.h"
void UAnimNotifyParryEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    ASRCharacter* PlayerCharacter = Cast<ASRCharacter>(MeshComp->GetOwner());
    if (IsValid(PlayerCharacter))
    {
        PlayerCharacter->OnCombatAnimationEnd();
        PlayerCharacter->CombatComponent->bIsGuarding = false;
        PlayerCharacter->CombatComponent->bIsParrying = false;
        PlayerCharacter->CombatComponent->bIsAttacking = false;
        PlayerCharacter->SetCharacterState(ECharacterState::Idle);
    }
}
