// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h" // FDamageEvent 포함
#include "SRCharacter.h"
#include "PlayerStats.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Weapon"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	HitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCapsule"));
	HitCapsule->SetupAttachment(WeaponMesh);

	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	DisableHit();

	HitCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponOverlap);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, AttackMontage);
	DOREPLIFETIME(AWeapon, GuardMontage);
	DOREPLIFETIME(AWeapon, ParryMontage);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::EnableHit()
{
	HitCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DisableHit()
{
	HitCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// [수정] OnWeaponOverlap 함수를 아래 내용으로 교체합니다.
void AWeapon::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASRCharacter* MyOwner = Cast<ASRCharacter>(GetOwner());

	// 이 무기의 주인이 없거나, 서버 권한을 가지고 있지 않다면(즉, 클라이언트의 무기라면)
	// 아무것도 하지 않고 즉시 함수를 종료합니다.
	if (!MyOwner || !MyOwner->HasAuthority())
	{
		return;
	}

	// --- 아래 로직은 이제 오직 서버에서만 실행됩니다 ---

	if (!OtherActor || OtherActor == this || OtherActor == MyOwner) return;

	if (HitActors.Contains(OtherActor)) return;

	ASRCharacter* HitCharacter = Cast<ASRCharacter>(OtherActor);
	if (!IsValid(HitCharacter)) return;

	// 서버에서만 로그를 남기도록 하여 확인 용이
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Weapon Hit on SERVER: %s by %s"), *HitCharacter->GetName(), *MyOwner->GetName()), true, true, FLinearColor::Green, 3.0f);

	HitActors.Add(OtherActor);

	// 서버이므로 직접 데미지 처리 함수를 호출합니다.
	ApplyDamageToTarget(HitCharacter);
}

void AWeapon::StartAttack()
{
	HitActors.Empty();
	EnableHit();
}

void AWeapon::EndAttack()
{
	DisableHit();
}

// [수정] ApplyDamageToTarget 함수는 그대로 사용합니다.
void AWeapon::ApplyDamageToTarget(ASRCharacter* Target)
{
	if (!IsValid(Target)) return;

	ASRCharacter* Attacker = Cast<ASRCharacter>(GetOwner());
	if (!IsValid(Attacker)) return;

	// 안전한 데미지 계산
	float SafeDamage = 1.0f; // 기본 데미지

	if (IsValid(Attacker->PlayerStat))
	{
		SafeDamage = FMath::Clamp(Attacker->PlayerStat->Attack, 1.0f, 100.0f);
	}

	UE_LOG(LogTemp, Warning, TEXT("Safe damage calculated by server: %.2f"), SafeDamage);

	// 데미지 적용
	FDamageEvent DamageEvent;
	Target->TakeDamage(SafeDamage, DamageEvent, Attacker->GetController(), Attacker);
}