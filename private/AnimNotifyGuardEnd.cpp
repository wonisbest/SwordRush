// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyGuardEnd.h"
#include "SRCharacter.h"
#include "CombatActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyGuardEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    ASRCharacter* PlayerCharacter = Cast<ASRCharacter>(MeshComp->GetOwner());
    if (IsValid(PlayerCharacter))
    {
        PlayerCharacter->OnCombatAnimationEnd();
        PlayerCharacter->CombatComponent->bIsGuarding = false;
        PlayerCharacter->CombatComponent->bIsAttacking = false;
        PlayerCharacter->CombatComponent->bIsParrying = false;
        PlayerCharacter->SetCharacterState(ECharacterState::Idle);
        //UKismetSystemLibrary::PrintString(this, TEXT("Guard Off"), true, true, FLinearColor::Blue, 3.0f);
    }
}
