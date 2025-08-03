// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveSkillDash.h"
#include "SRCharacter.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"

void UActiveSkillDash::ActivateSkill(ASRCharacter* Character)
{
    Super::ActivateSkill(Character);
	if (!Character) return;

	StartLocation = Character->GetActorLocation();
	Direction = Character->GetActorForwardVector();
	DashDistance = 500.f; //10m = 1000cm
	DashTime = 0.5f;
	ElapsedTime = 0.f;

    Character->GetWorldTimerManager().SetTimer(DashTimerHandle, FTimerDelegate::CreateUObject(this, &UActiveSkillDash::HandleDash, Character), 0.01f, true);
}

void UActiveSkillDash::HandleDash(ASRCharacter* Character)
{
    if (!Character) return;

    ElapsedTime += 0.01f;
    float Alpha = ElapsedTime / DashTime;
    Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

    FVector MoveDirection = Direction.GetSafeNormal();
    FVector TargetOffset = MoveDirection * (DashDistance * Alpha);
    FVector TargetLocation = StartLocation + TargetOffset;

    // 1. ���� ��ġ ����
    FVector TraceStart = TargetLocation + FVector(0.f, 0.f, 100.f);
    FVector TraceEnd = TargetLocation - FVector(0.f, 0.f, 200.f);

    FHitResult GroundHit;
    bool bHitGround = Character->GetWorld()->LineTraceSingleByChannel(GroundHit, TraceStart, TraceEnd, ECC_Visibility);

    if (bHitGround)
    {
        float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        TargetLocation.Z = GroundHit.Location.Z + CapsuleHalfHeight;
    }

    // 2. �̵� �� ��ֹ� �浹 üũ (Sweep = true)
    FHitResult ObstacleHit;
    bool bBlocked = Character->SetActorLocation(TargetLocation, true, &ObstacleHit);

    if (!bBlocked || ObstacleHit.bBlockingHit)
    {
        // �� ���� ��ֹ��� ������ Dash ����
        Character->GetWorldTimerManager().ClearTimer(DashTimerHandle);
    }

    // 3. ������ ������ ��� ����
    if (Alpha >= 1.f)
    {
        Character->GetWorldTimerManager().ClearTimer(DashTimerHandle);
    }

}
