// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUserInterfaceWidget.h"
#include "Components/Button.h"
#include "SessionWidget.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void ULobbyUserInterfaceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SocialButton) SocialButton->OnClicked.AddDynamic(this, &ULobbyUserInterfaceWidget::OnSocialButtonClicked);
	if (BackButton) BackButton->OnClicked.AddDynamic(this, &ULobbyUserInterfaceWidget::OnBackButtonClicked);
	if (ReadyButton) ReadyButton->OnClicked.AddDynamic(this, &ULobbyUserInterfaceWidget::OnReadyButtonClicked);
}

void ULobbyUserInterfaceWidget::OnSocialButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Social button clicked"));

    if (bIsSocialOn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Closing SessionWidget"));
        if (CurrentSessionWidget)
        {
            CurrentSessionWidget->RemoveFromParent();
            CurrentSessionWidget = nullptr;
        }
        bIsSocialOn = false;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Opening SessionWidget"));

        if (!SessionWidgetClass)
        {
            UE_LOG(LogTemp, Error, TEXT("SessionWidgetClass is null! Cannot create SessionWidget"));
            return;
        }

        if (!SessionParentSizeBox)
        {
            UE_LOG(LogTemp, Error, TEXT("SessionParentSizeBox is null! Cannot add SessionWidget"));
            return;
        }

        if (!CurrentSessionWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("Creating SessionWidget..."));
            CurrentSessionWidget = CreateWidget<USessionWidget>(this, SessionWidgetClass);

            if (CurrentSessionWidget)
            {
                UE_LOG(LogTemp, Warning, TEXT("SessionWidget created successfully"));
                SessionParentSizeBox->AddChild(CurrentSessionWidget);
                bIsSocialOn = true;
                UE_LOG(LogTemp, Warning, TEXT("SessionWidget added to parent"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create SessionWidget"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SessionWidget already exists"));
        }
    }
}

void ULobbyUserInterfaceWidget::OnBackButtonClicked()
{
	if (bIsSocialOn)
	{
		if (CurrentSessionWidget)
		{
			CurrentSessionWidget->RemoveFromParent();
			CurrentSessionWidget = nullptr;
		}
		bIsSocialOn = false;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("Level_First"));
	
}

void ULobbyUserInterfaceWidget::OnReadyButtonClicked()
{
	
}
