// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MyGameInstance.h"

void UStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	SRChar = Cast<ASRCharacter>(PC->GetPawn());
	Stats = SRChar->PlayerStat;

	if (AttackUpButton) AttackUpButton->OnClicked.AddDynamic(this, &UStatsWidget::OnAttackUpClicked);
	if (AttackDownButton) AttackDownButton->OnClicked.AddDynamic(this, &UStatsWidget::OnAttackDownClicked);

	if (DefenseUpButton) DefenseUpButton->OnClicked.AddDynamic(this, &UStatsWidget::OnDefenseUpClicked);
	if (DefenseDownButton) DefenseDownButton->OnClicked.AddDynamic(this, &UStatsWidget::OnDefenseDownClicked);

	if (AttackSpeedUpButton) AttackSpeedUpButton->OnClicked.AddDynamic(this, &UStatsWidget::OnAttackSpeedUpClicked);
	if (AttackSpeedDownButton) AttackSpeedDownButton->OnClicked.AddDynamic(this, &UStatsWidget::OnAttackSpeedDownClicked);

	if (MoveSpeedUpButton) MoveSpeedUpButton->OnClicked.AddDynamic(this, &UStatsWidget::OnMoveSpeedUpClicked);
	if (MoveSpeedDownButton) MoveSpeedDownButton->OnClicked.AddDynamic(this, &UStatsWidget::OnMoveSpeedDownClicked);

	if (BackButton) BackButton->OnClicked.AddDynamic(this, &UStatsWidget::OnBackClicked);
	if (SaveButton) SaveButton->OnClicked.AddDynamic(this, &UStatsWidget::OnSaveClicked);
	if (StatResetButton) StatResetButton->OnClicked.AddDynamic(this, &UStatsWidget::OnStatResetClicked);
	if (WeaponSkillButton) WeaponSkillButton->OnClicked.AddDynamic(this, &UStatsWidget::OnWeaponSkillClicked);

	UpdateStatText();
}

void UStatsWidget::UpdateStatText()
{
	if (!Stats) return;

	if (TextAttack) TextAttack->SetText(FText::AsNumber(Stats->Attack));
	if (TextDefense) TextDefense->SetText(FText::AsNumber(Stats->Defense));
	if (TextAttackSpeed) TextAttackSpeed->SetText(FText::AsNumber(Stats->AttackSpeed));
	if (TextMoveSpeed) TextMoveSpeed->SetText(FText::AsNumber(Stats->MoveSpeed));
	if (TextStatPoint) TextStatPoint->SetText(FText::AsNumber(Stats->StatPoint));
}



void UStatsWidget::OnAttackUpClicked()
{
	if (!Stats) return;
	if (Stats->StatPoint > 0)
	{
		Stats->Attack++;
		Stats->StatPoint--;
		UpdateStatText();
	}
}

void UStatsWidget::OnAttackDownClicked()
{
	if (!Stats) return;
	if (Stats->Attack > 1)
	{
		Stats->Attack--;
		Stats->StatPoint++;
		UpdateStatText();
	}
}

void UStatsWidget::OnDefenseUpClicked()
{
	if (!Stats) return;
	if (Stats->StatPoint > 0)
	{
		Stats->Defense++;
		Stats->StatPoint--;
		UpdateStatText();
	}
}

void UStatsWidget::OnDefenseDownClicked()
{
	if (!Stats) return;
	if (Stats->Defense > 1)
	{
		Stats->Defense--;
		Stats->StatPoint++;
		UpdateStatText();
	}
}

void UStatsWidget::OnAttackSpeedUpClicked()
{
	if (!Stats) return;
	if (Stats->StatPoint > 0)
	{
		Stats->AttackSpeed++;
		Stats->StatPoint--;
		UpdateStatText();
	}
}

void UStatsWidget::OnAttackSpeedDownClicked()
{
	if (!Stats) return;
	if (Stats->AttackSpeed > 1)
	{
		Stats->AttackSpeed--;
		Stats->StatPoint++;
		UpdateStatText();
	}
}

void UStatsWidget::OnMoveSpeedUpClicked()
{
	if (!Stats) return;
	if (Stats->StatPoint > 0)
	{
		Stats->MoveSpeed++;
		Stats->StatPoint--;
		UpdateStatText();
	}
}

void UStatsWidget::OnMoveSpeedDownClicked()
{
	if (!Stats) return;
	if (Stats->MoveSpeed > 1)
	{
		Stats->MoveSpeed--;
		Stats->StatPoint++;
		UpdateStatText();
	}
}
	

void UStatsWidget::OnBackClicked()
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

void UStatsWidget::OnSaveClicked()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->CurrentStats.AttackPoint = Stats->Attack;
		GI->CurrentStats.DeffensePoint = Stats->Defense;
		GI->CurrentStats.AttackSpeedPoint = Stats->AttackSpeed;
		GI->CurrentStats.MoveSpeedPoint = Stats->MoveSpeed;
		GI->CurrentStats.StatPoint = Stats->StatPoint;
		GI->SavePlayerData();
	}
}

void UStatsWidget::OnStatResetClicked()
{
	Stats->Attack = 1;
	Stats->Defense = 1;
	Stats->AttackSpeed = 1;
	Stats->MoveSpeed = 1;
	Stats->StatPoint = 15;

	UpdateStatText();
}

void UStatsWidget::OnWeaponSkillClicked()
{
	RemoveFromParent();

	if (WeaponSkillWidgetClass)
	{
		UUserWidget* WeaponSkills = CreateWidget<UUserWidget>(GetWorld(), WeaponSkillWidgetClass);
		if (WeaponSkills)
		{
			WeaponSkills->AddToViewport();
		}
	}
}
