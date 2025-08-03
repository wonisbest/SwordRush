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

    // 1. 지면 위치 보정
    FVector TraceStart = TargetLocation + FVector(0.f, 0.f, 100.f);
    FVector TraceEnd = TargetLocation - FVector(0.f, 0.f, 200.f);

    FHitResult GroundHit;
    bool bHitGround = Character->GetWorld()->LineTraceSingleByChannel(GroundHit, TraceStart, TraceEnd, ECC_Visibility);

    if (bHitGround)
    {
        float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        TargetLocation.Z = GroundHit.Location.Z + CapsuleHalfHeight;
    }

    // 2. 이동 전 장애물 충돌 체크 (Sweep = true)
    FHitResult ObstacleHit;
    bool bBlocked = Character->SetActorLocation(TargetLocation, true, &ObstacleHit);

    if (!bBlocked || ObstacleHit.bBlockingHit)
    {
        // 벽 같은 장애물에 막히면 Dash 종료
        Character->GetWorldTimerManager().ClearTimer(DashTimerHandle);
    }

    // 3. 끝까지 도달한 경우 종료
    if (Alpha >= 1.f)
    {
        Character->GetWorldTimerManager().ClearTimer(DashTimerHandle);
    }

}
