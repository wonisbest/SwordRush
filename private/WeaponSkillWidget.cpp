// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSkillWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SkillList.h"
#include "MyGameInstance.h"
#include "SkillActorComponent.h"

void UWeaponSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	SRChar = Cast<ASRCharacter>(PC->GetPawn());
	//UI_WeaponIdx = SRChar->WeaponIdx;
	SetWeaponNameText();
	SetSkillNameText();
	SetSkillExplainText();
	if (WeaponPrevButton) WeaponPrevButton->OnClicked.AddDynamic(this, &UWeaponSkillWidget::OnWeaponPrevClicked);
	if (WeaponNextButton) WeaponNextButton->OnClicked.AddDynamic(this, &UWeaponSkillWidget::OnWeaponNextClicked);

	if (SkillPrevButton) SkillPrevButton->OnClicked.AddDynamic(this, &UWeaponSkillWidget::OnSkillPrevClicked);
	if (SkillNextButton) SkillNextButton->OnClicked.AddDynamic(this, &UWeaponSkillWidget::OnSkillNextClicked);

	if (BackButton) BackButton->OnClicked.AddDynamic(this, &UWeaponSkillWidget::OnBackClicked);
	if (SaveButton) SaveButton->OnClicked.AddDynamic(this, &UWeaponSkillWidget::OnSaveClicked);
}

void UWeaponSkillWidget::OnWeaponPrevClicked()
{
	if (SRChar->WeaponIdx > 0)
	{
		SRChar->WeaponIdx--;
	}
	else
	{
		SRChar->WeaponIdx = SRChar->BPArray_Weapons.Num() - 1;
	}

	SRChar->SetWeaponMesh();
	SetWeaponNameText();
}

void UWeaponSkillWidget::OnWeaponNextClicked()
{
	if (SRChar->WeaponIdx < SRChar->BPArray_Weapons.Num() - 1)
	{
		SRChar->WeaponIdx++;
	}
	else
	{
		SRChar->WeaponIdx = 0;
	}

	SRChar->SetWeaponMesh();
	SetWeaponNameText();
}

void UWeaponSkillWidget::OnSkillPrevClicked()
{
	if (SRChar->SkillComponent->SkillsListIdx > 0)
	{
		SRChar->SkillComponent->SkillsListIdx--;
	}
	else
	{
		SRChar->SkillComponent->SkillsListIdx = SRChar->SkillComponent->SkillsList->Skills.Num() - 1;
	}
	SetSkillNameText();
	SetSkillExplainText();
}

void UWeaponSkillWidget::OnSkillNextClicked()
{
	if (SRChar->SkillComponent->SkillsListIdx < SRChar->SkillComponent->SkillsList->Skills.Num() - 1)
	{
		SRChar->SkillComponent->SkillsListIdx++;
	}
	else
	{
		SRChar->SkillComponent->SkillsListIdx = 0;
	}

	SetSkillNameText();
	SetSkillExplainText();
}

void UWeaponSkillWidget::OnBackClicked()
{
	RemoveFromParent();

	if (StatsWidgetClass)
	{
		UUserWidget* Stat = CreateWidget<UUserWidget>(GetWorld(), StatsWidgetClass);
		if (Stat)
		{
			Stat->AddToViewport();
		}
	}
}

void UWeaponSkillWidget::OnSaveClicked()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->CurrentWeaponSet.WeaponIndex = SRChar->WeaponIdx;
		GI->CurrentSkillSet.SkillIndex = SRChar->SkillComponent->SkillsListIdx;
		GI->SavePlayerData();
		SRChar->SkillComponent->CurrentSkill = SRChar->SkillComponent->SkillsList->Skills[SRChar->SkillComponent->SkillsListIdx];
		UE_LOG(LogTemp, Log, TEXT("WeaponIdx : %d"), GI->CurrentWeaponSet.WeaponIndex);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("GI is nope"));
	}
	
}

void UWeaponSkillWidget::SetWeaponNameText()
{
	if (!SRChar) return;

	// 현재 선택된 무기 클래스 가져오기
	if (SRChar->BPArray_Weapons.IsValidIndex(SRChar->WeaponIdx))
	{
		TSubclassOf<AWeapon> WeaponClass = SRChar->BPArray_Weapons[SRChar->WeaponIdx];
		if (WeaponClass)
		{
			// 기본 객체에서 WeaponName 가져오기
			AWeapon* DefaultWeapon = WeaponClass->GetDefaultObject<AWeapon>();
			if (DefaultWeapon)
			{
				WeaponNameTextBlock->SetText(FText::FromName(DefaultWeapon->WeaponName));
			}
		}
	}
	else
	{
		WeaponNameTextBlock->SetText(FText::FromString(TEXT("Invalid Weapon")));
	}
}

void UWeaponSkillWidget::SetSkillNameText()
{
	if (!IsValid(SRChar->SkillComponent->CurrentSkill)) return;
	SkillNameTextBlock->SetText(FText::FromName(SRChar->SkillComponent->SkillsList->Skills[SRChar->SkillComponent->SkillsListIdx]->SkillName));
}

void UWeaponSkillWidget::SetSkillExplainText()
{
	if (!IsValid(SRChar->SkillComponent->CurrentSkill)) return;
	SkillExplainTextBlock->SetText(SRChar->SkillComponent->SkillsList->Skills[SRChar->SkillComponent->SkillsListIdx]->SkillDescription);
}
