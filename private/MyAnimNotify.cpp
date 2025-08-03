// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimNotify.h"
#include "SRCharacter.h"


void UMyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (ASRCharacter* Character = Cast<ASRCharacter>(MeshComp->GetOwner()))
    {
        Character->OnComboInputWindowOpened(true); // bool ���̳� ���� �ӽ� �� ���
    }
}
