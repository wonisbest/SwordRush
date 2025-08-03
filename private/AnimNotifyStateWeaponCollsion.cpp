// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyStateWeaponCollsion.h"
#include "SRCharacter.h"
#include "Weapon.h"

void UAnimNotifyStateWeaponCollsion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ASRCharacter* PlayerCharacter = Cast<ASRCharacter>(MeshComp->GetOwner()))
	{
		if (AWeapon* EquippedWeapon = PlayerCharacter->GetEquippedWeapon())
		{
			EquippedWeapon->StartAttack();
		}
	}
}

void UAnimNotifyStateWeaponCollsion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ASRCharacter* PlayerCharacter = Cast<ASRCharacter>(MeshComp->GetOwner()))
	{
		if (AWeapon* EquippedWeapon = PlayerCharacter->GetEquippedWeapon())
		{
			EquippedWeapon->EndAttack();
		}
	}
}
