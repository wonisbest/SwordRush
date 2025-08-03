// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimNotifyEnd.h"
#include "SRCharacter.h"
#include "CombatActorComponent.h"

void UMyAnimNotifyEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    ASRCharacter* PlayerCharacter = Cast<ASRCharacter>(MeshComp->GetOwner());
    if (IsValid(PlayerCharacter))
    {
        PlayerCharacter->OnCombatAnimationEnd();
        PlayerCharacter->CombatComponent->ResetCombo();
    }
}
