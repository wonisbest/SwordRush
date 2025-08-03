// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifySkillEnd.h"
#include "SRCharacter.h"

void UAnimNotifySkillEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ASRCharacter* PlayerCharacter = Cast<ASRCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter)) PlayerCharacter->SetCharacterState(ECharacterState::Idle);
}
