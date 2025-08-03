// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizeWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "MyGameInstance.h"
#include "AppearanceActorComponent.h"

void UCustomizeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	SRChar = Cast<ASRCharacter>(PC->GetPawn());

 
	if (HatPrevButton) HatPrevButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnHatPrevClicked);
	if (HatNextButton) HatNextButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnHatNextClicked);

	if (EyePrevButton) EyePrevButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnEyePrevClicked);
	if (EyeNextButton) EyeNextButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnEyeNextClicked);


	if (FacePrevButton) FacePrevButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnFacePrevClicked);
	if (FaceNextButton) FaceNextButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnFaceNextClicked);


	if (NosePrevButton) NosePrevButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnNosePrevClicked);
	if (NoseNextButton) NoseNextButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnNoseNextClicked);


	if (TopPrevButton) TopPrevButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnTopPrevClicked);
	if (TopNextButton) TopNextButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnTopNextClicked);


	if (BottomPrevButton) BottomPrevButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnBottomPrevClicked);
	if (BottomNextButton) BottomNextButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnBottomNextClicked);


	if (GlovesPrevButton) GlovesPrevButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnGlovesPrevClicked);
	if (GlovesNextButton) GlovesNextButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnGlovesNextClicked);


	if (ShoesPrevButton) ShoesPrevButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnShoesPrevClicked);
	if (ShoesNextButton) ShoesNextButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnShoesNextClicked);

	if (BackButton) BackButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnBackClicked);
	if (SaveButton) SaveButton->OnClicked.AddDynamic(this, &UCustomizeWidget::OnSaveClicked);
}

void UCustomizeWidget::OnHatPrevClicked()
{
	if (SRChar->AppearanceComponent->HatIdx > 0)
	{
		SRChar->AppearanceComponent->HatIdx--;
	}
	else
	{
		SRChar->AppearanceComponent->HatIdx = SRChar->AppearanceComponent->SMArray_Hat.Num() - 1;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnHatNextClicked()
{
	if (SRChar->AppearanceComponent->HatIdx < SRChar->AppearanceComponent->SMArray_Hat.Num() - 1)
	{
		SRChar->AppearanceComponent->HatIdx++;
	}
	else
	{
		SRChar->AppearanceComponent->HatIdx = 0;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnEyePrevClicked()
{
	if (SRChar->AppearanceComponent->EyeIdx > 0)
	{
		SRChar->AppearanceComponent->EyeIdx--;
	}
	else
	{
		SRChar->AppearanceComponent->EyeIdx = SRChar->AppearanceComponent->SMArray_Eye.Num() - 1;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnEyeNextClicked()
{
	if (SRChar->AppearanceComponent->EyeIdx < SRChar->AppearanceComponent->SMArray_Eye.Num() - 1)
	{
		SRChar->AppearanceComponent->EyeIdx++;
	}
	else
	{
		SRChar->AppearanceComponent->EyeIdx = 0;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnFacePrevClicked()
{
	if (SRChar->AppearanceComponent->FaceIdx > 0)
	{
		SRChar->AppearanceComponent->FaceIdx--;
	}
	else
	{
		SRChar->AppearanceComponent->FaceIdx = SRChar->AppearanceComponent->SMArray_Face.Num() - 1;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnFaceNextClicked()
{
	if (SRChar->AppearanceComponent->FaceIdx < SRChar->AppearanceComponent->SMArray_Face.Num() - 1)
	{
		SRChar->AppearanceComponent->FaceIdx++;
	}
	else
	{
		SRChar->AppearanceComponent->FaceIdx = 0;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnNosePrevClicked()
{
	if (SRChar->AppearanceComponent->NoseIdx > 0)
	{
		SRChar->AppearanceComponent->NoseIdx--;
	}
	else
	{
		SRChar->AppearanceComponent->NoseIdx = SRChar->AppearanceComponent->SMArray_Nose.Num() - 1;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnNoseNextClicked()
{
	if (SRChar->AppearanceComponent->NoseIdx < SRChar->AppearanceComponent->SMArray_Nose.Num() - 1)
	{
		SRChar->AppearanceComponent->NoseIdx++;
	}
	else
	{
		SRChar->AppearanceComponent->NoseIdx = 0;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnTopPrevClicked()
{
	if (SRChar->AppearanceComponent->TopIdx > 0)
	{
		SRChar->AppearanceComponent->TopIdx--;
	}
	else
	{
		SRChar->AppearanceComponent->TopIdx = SRChar->AppearanceComponent->SMArray_Top.Num() - 1;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnTopNextClicked()
{
	if (SRChar->AppearanceComponent->TopIdx < SRChar->AppearanceComponent->SMArray_Top.Num() - 1)
	{
		SRChar->AppearanceComponent->TopIdx++;
	}
	else
	{
		SRChar->AppearanceComponent->TopIdx = 0;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnBottomPrevClicked()
{
	if (SRChar->AppearanceComponent->BottomIdx > 0)
	{
		SRChar->AppearanceComponent->BottomIdx--;
	}
	else
	{
		SRChar->AppearanceComponent->BottomIdx = SRChar->AppearanceComponent->SMArray_Bottom.Num() - 1;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnBottomNextClicked()
{
	if (SRChar->AppearanceComponent->BottomIdx < SRChar->AppearanceComponent->SMArray_Bottom.Num() - 1)
	{
		SRChar->AppearanceComponent->BottomIdx++;
	}
	else
	{
		SRChar->AppearanceComponent->BottomIdx = 0;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnGlovesPrevClicked()
{
	if (SRChar->AppearanceComponent->GlovesIdx > 0)
	{
		SRChar->AppearanceComponent->GlovesIdx--;
	}
	else
	{
		SRChar->AppearanceComponent->GlovesIdx = SRChar->AppearanceComponent->SMArray_Gloves.Num() - 1;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnGlovesNextClicked()
{
	if (SRChar->AppearanceComponent->GlovesIdx < SRChar->AppearanceComponent->SMArray_Gloves.Num() - 1)
	{
		SRChar->AppearanceComponent->GlovesIdx++;
	}
	else
	{
		SRChar->AppearanceComponent->GlovesIdx = 0;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnShoesPrevClicked()
{
	if (SRChar->AppearanceComponent->ShoesIdx > 0)
	{
		SRChar->AppearanceComponent->ShoesIdx--;
	}
	else
	{
		SRChar->AppearanceComponent->ShoesIdx = SRChar->AppearanceComponent->SMArray_Shoes.Num() - 1;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnShoesNextClicked()
{
	if (SRChar->AppearanceComponent->ShoesIdx < SRChar->AppearanceComponent->SMArray_Shoes.Num() - 1)
	{
		SRChar->AppearanceComponent->ShoesIdx++;
	}
	else
	{
		SRChar->AppearanceComponent->ShoesIdx = 0;
	}
	SRChar->AppearanceComponent->SetClothMesh();
}

void UCustomizeWidget::OnBackClicked()
{
	RemoveFromParent();

	if (LobbyWidgetClass)
	{
		UUserWidget* Lobby = CreateWidget<UUserWidget>(GetWorld(), LobbyWidgetClass);
		if (Lobby)
		{
			Lobby->AddToViewport();
		}
	}
}

void UCustomizeWidget::OnSaveClicked()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		UAppearanceActorComponent* AppComp = SRChar->AppearanceComponent;
		GI->CurrentPartIndex.HatIndex = SRChar->AppearanceComponent->HatIdx;
		GI->CurrentPartIndex.EyeIndex = SRChar->AppearanceComponent->EyeIdx;
		GI->CurrentPartIndex.FaceIndex = SRChar->AppearanceComponent->FaceIdx;
		GI->CurrentPartIndex.NoseIndex = SRChar->AppearanceComponent->NoseIdx;
		GI->CurrentPartIndex.TopIndex = SRChar->AppearanceComponent->TopIdx;
		GI->CurrentPartIndex.BottomIndex = SRChar->AppearanceComponent->BottomIdx;
		GI->CurrentPartIndex.GlovesIndex = SRChar->AppearanceComponent->GlovesIdx;
		GI->CurrentPartIndex.ShoesIndex = SRChar->AppearanceComponent->ShoesIdx;

		GI->SavePlayerData();
	}
}
