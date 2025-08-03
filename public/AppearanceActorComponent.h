// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "AppearanceActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDRUSH_API UAppearanceActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAppearanceActorComponent();

    // 의상 메시 컴포넌트들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
    USkeletalMeshComponent* HatMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* EyeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FaceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* NoseMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
    USkeletalMeshComponent* TopMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
    USkeletalMeshComponent* BottomMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GlovesMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
    USkeletalMeshComponent* ShoesMesh;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMesh*> SMArray_Hat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMesh*> SMArray_Eye;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMesh*> SMArray_Nose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMesh*> SMArray_Face;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMesh*> SMArray_Top;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMesh*> SMArray_Bottom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMesh*> SMArray_Gloves;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMesh*> SMArray_Shoes;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	int HatIdx;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	int EyeIdx;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	int FaceIdx;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	int NoseIdx;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	int TopIdx;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	int BottomIdx;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	int GlovesIdx;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Clothes", meta = (AllowPrivateAccess = "true"))
	int ShoesIdx;

	// 의상 변경 함수
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void ChangeClothes(int32 NewHatIndex, int32 NewTopIndex, int32 NewBottomIndex);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetClothMesh();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	class ASRCharacter* OwnerCharacter;
	void InitializeMeshComponents();
};
