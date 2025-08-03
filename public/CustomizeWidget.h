// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SRCharacter.h"
#include "CustomizeWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API UCustomizeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	ACharacter* Character;
	ASRCharacter* SRChar;
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* HatPrevButton;
	UPROPERTY(meta = (BindWidget))
	UButton* HatNextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* EyePrevButton;
	UPROPERTY(meta = (BindWidget))
	UButton* EyeNextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FacePrevButton;
	UPROPERTY(meta = (BindWidget))
	UButton* FaceNextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NosePrevButton;
	UPROPERTY(meta = (BindWidget))
	UButton* NoseNextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* TopPrevButton;
	UPROPERTY(meta = (BindWidget))
	UButton* TopNextButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BottomPrevButton;
	UPROPERTY(meta = (BindWidget))
	UButton* BottomNextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* GlovesPrevButton;
	UPROPERTY(meta = (BindWidget))
	UButton* GlovesNextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ShoesPrevButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ShoesNextButton;

	//UPROPERTY(meta = (BindWidget))
	//UButton* WeaponPrevButton;
	//UPROPERTY(meta = (BindWidget))
	//UButton* NextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UFUNCTION()
	void OnHatPrevClicked();

	UFUNCTION()
	void OnHatNextClicked();

	UFUNCTION()
	void OnEyePrevClicked();

	UFUNCTION()
	void OnEyeNextClicked();

	UFUNCTION()
	void OnFacePrevClicked();

	UFUNCTION()
	void OnFaceNextClicked();

	UFUNCTION()
	void OnNosePrevClicked();

	UFUNCTION()
	void OnNoseNextClicked();

	UFUNCTION()
	void OnTopPrevClicked();

	UFUNCTION()
	void OnTopNextClicked();

	UFUNCTION()
	void OnBottomPrevClicked();

	UFUNCTION()
	void OnBottomNextClicked();

	UFUNCTION()
	void OnGlovesPrevClicked();

	UFUNCTION()
	void OnGlovesNextClicked();

	UFUNCTION()
	void OnShoesPrevClicked();

	UFUNCTION()
	void OnShoesNextClicked();

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnSaveClicked();
};
