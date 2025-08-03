// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h" // FDamageEvent ����
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

// [����] OnWeaponOverlap �Լ��� �Ʒ� �������� ��ü�մϴ�.
void AWeapon::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASRCharacter* MyOwner = Cast<ASRCharacter>(GetOwner());

	// �� ������ ������ ���ų�, ���� ������ ������ ���� �ʴٸ�(��, Ŭ���̾�Ʈ�� ������)
	// �ƹ��͵� ���� �ʰ� ��� �Լ��� �����մϴ�.
	if (!MyOwner || !MyOwner->HasAuthority())
	{
		return;
	}

	// --- �Ʒ� ������ ���� ���� ���������� ����˴ϴ� ---

	if (!OtherActor || OtherActor == this || OtherActor == MyOwner) return;

	if (HitActors.Contains(OtherActor)) return;

	ASRCharacter* HitCharacter = Cast<ASRCharacter>(OtherActor);
	if (!IsValid(HitCharacter)) return;

	// ���������� �α׸� ���⵵�� �Ͽ� Ȯ�� ����
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Weapon Hit on SERVER: %s by %s"), *HitCharacter->GetName(), *MyOwner->GetName()), true, true, FLinearColor::Green, 3.0f);

	HitActors.Add(OtherActor);

	// �����̹Ƿ� ���� ������ ó�� �Լ��� ȣ���մϴ�.
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

// [����] ApplyDamageToTarget �Լ��� �״�� ����մϴ�.
void AWeapon::ApplyDamageToTarget(ASRCharacter* Target)
{
	if (!IsValid(Target)) return;

	ASRCharacter* Attacker = Cast<ASRCharacter>(GetOwner());
	if (!IsValid(Attacker)) return;

	// ������ ������ ���
	float SafeDamage = 1.0f; // �⺻ ������

	if (IsValid(Attacker->PlayerStat))
	{
		SafeDamage = FMath::Clamp(Attacker->PlayerStat->Attack, 1.0f, 100.0f);
	}

	UE_LOG(LogTemp, Warning, TEXT("Safe damage calculated by server: %.2f"), SafeDamage);

	// ������ ����
	FDamageEvent DamageEvent;
	Target->TakeDamage(SafeDamage, DamageEvent, Attacker->GetController(), Attacker);
}