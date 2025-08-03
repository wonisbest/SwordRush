// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyCheckNextCombo.h"
#include "SRCharacter.h"
#include "CombatActorComponent.h"
void UAnimNotifyCheckNextCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    ASRCharacter* PlayerCharacter = Cast<ASRCharacter>(MeshComp->GetOwner());
    if (IsValid(PlayerCharacter))
    {
        PlayerCharacter->CombatComponent->NextComboAttack(); // 새로 만든 함수 호출
    }
	
}
