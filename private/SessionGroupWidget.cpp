// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionGroupWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "MyGameInstance.h"
void USessionGroupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (JoinButton) JoinButton->OnClicked.AddDynamic(this, &USessionGroupWidget::OnJoinClicked);
}

void USessionGroupWidget::OnJoinClicked()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UMyGameInstance* MyGI = Cast<UMyGameInstance>(GameInstance))
		{
			//MyGI->JoinAdvancedGameSessionByIndex(0);
		}
	}

}

