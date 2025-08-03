// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSoundAnimNotify.h"
#include "SRCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"

void UAttackSoundAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

    ASRCharacter* PlayerCharacter = Cast<ASRCharacter>(MeshComp->GetOwner());
    if (IsValid(PlayerCharacter))
    {
        AWeapon* EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
        if (IsValid(EquippedWeapon) && IsValid(EquippedWeapon->AttackSound))
        {
            UGameplayStatics::PlaySound2D(GetWorld(), EquippedWeapon->AttackSound);
        }
    }
}
