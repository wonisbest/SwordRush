// Fill out your copyright notice in the Description page of Project Settings.


#include "AppearanceActorComponent.h"
#include "SRCharacter.h"
#include "Net/UnrealNetwork.h"  // DOREPLIFETIME을 위해 필요
#include "Components/SkeletalMeshComponent.h"  // 추가

// Sets default values for this component's properties
UAppearanceActorComponent::UAppearanceActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HatIdx = 0;
	EyeIdx = 0;
	FaceIdx = 0;
	NoseIdx = 0;
	TopIdx = 0;
	BottomIdx = 0;
	GlovesIdx = 0;
	ShoesIdx = 0;
	// ...
}


// Called when the game starts
void UAppearanceActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ASRCharacter>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		// Owner Character의 메시 컴포넌트들과 연결
		InitializeMeshComponents();
		SetClothMesh(); // 초기 의상 설정
	}
}


// Called every frame
void UAppearanceActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAppearanceActorComponent::ChangeClothes(int32 NewHatIndex, int32 NewTopIndex, int32 NewBottomIndex)
{
}

void UAppearanceActorComponent::SetClothMesh()
{
	if (!IsValid(this))
	{
		UE_LOG(LogTemp, Error, TEXT("AppearanceActorComponent is invalid"));
		return;
	}

	if (!IsValid(OwnerCharacter))
	{
		OwnerCharacter = Cast<ASRCharacter>(GetOwner());
		UE_LOG(LogTemp, Error, TEXT("AppearanceActorComponent: Is Not Valid OwnerCharacter"));
		return;
	}

	// 메시 컴포넌트 null 체크
	USkeletalMeshComponent* MeshComp = OwnerCharacter->GetMesh();
	if (!IsValid(MeshComp))
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh component is null in SetClothMesh"));
		return;
	}

	if (HatMesh && SMArray_Hat.IsValidIndex(HatIdx))
	{
		HatMesh->SetSkeletalMesh(SMArray_Hat[HatIdx]);
		HatMesh->SetLeaderPoseComponent(OwnerCharacter->GetMesh());
	}

	if (EyeMesh && SMArray_Eye.IsValidIndex(EyeIdx))
	{
		EyeMesh->SetSkeletalMesh(SMArray_Eye[EyeIdx]);
		EyeMesh->SetLeaderPoseComponent(OwnerCharacter->GetMesh());
	}

	if (FaceMesh && SMArray_Face.IsValidIndex(FaceIdx))
	{
		FaceMesh->SetSkeletalMesh(SMArray_Face[FaceIdx]);
		FaceMesh->SetLeaderPoseComponent(OwnerCharacter->GetMesh());
	}

	if (NoseMesh && SMArray_Nose.IsValidIndex(NoseIdx))
	{
		NoseMesh->SetSkeletalMesh(SMArray_Nose[NoseIdx]);
		NoseMesh->SetLeaderPoseComponent(OwnerCharacter->GetMesh());
	}

	if (TopMesh && SMArray_Top.IsValidIndex(TopIdx))
	{
		TopMesh->SetSkeletalMesh(SMArray_Top[TopIdx]);
		TopMesh->SetLeaderPoseComponent(OwnerCharacter->GetMesh());
	}

	if (BottomMesh && SMArray_Bottom.IsValidIndex(BottomIdx))
	{
		BottomMesh->SetSkeletalMesh(SMArray_Bottom[BottomIdx]);
		BottomMesh->SetLeaderPoseComponent(OwnerCharacter->GetMesh());
	}

	if (GlovesMesh && SMArray_Gloves.IsValidIndex(GlovesIdx))
	{
		GlovesMesh->SetSkeletalMesh(SMArray_Gloves[GlovesIdx]);
		GlovesMesh->SetLeaderPoseComponent(OwnerCharacter->GetMesh());
	}

	if (ShoesMesh && SMArray_Shoes.IsValidIndex(ShoesIdx))
	{
		ShoesMesh->SetSkeletalMesh(SMArray_Shoes[ShoesIdx]);
		ShoesMesh->SetLeaderPoseComponent(OwnerCharacter->GetMesh());
	}
}

void UAppearanceActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAppearanceActorComponent, HatIdx);
	DOREPLIFETIME(UAppearanceActorComponent, EyeIdx);
	DOREPLIFETIME(UAppearanceActorComponent, FaceIdx);
	DOREPLIFETIME(UAppearanceActorComponent, NoseIdx);
	DOREPLIFETIME(UAppearanceActorComponent, TopIdx);
	DOREPLIFETIME(UAppearanceActorComponent, BottomIdx);
	DOREPLIFETIME(UAppearanceActorComponent, GlovesIdx);
	DOREPLIFETIME(UAppearanceActorComponent, ShoesIdx);
}

void UAppearanceActorComponent::InitializeMeshComponents()
{
	if (!IsValid(OwnerCharacter)) return;

	HatMesh = OwnerCharacter->HatMesh;
	EyeMesh = OwnerCharacter->EyeMesh;
	FaceMesh = OwnerCharacter->FaceMesh;
	NoseMesh = OwnerCharacter->NoseMesh;
	TopMesh = OwnerCharacter->TopMesh;
	BottomMesh = OwnerCharacter->BottomMesh;
	GlovesMesh = OwnerCharacter->GlovesMesh;
	ShoesMesh = OwnerCharacter->ShoesMesh;
}

