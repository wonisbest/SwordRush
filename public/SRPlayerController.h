// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SRPlayerController.generated.h"

class UGameSetWidget;
/**
 * 
 */
UCLASS()
class SWORDRUSH_API ASRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    ASRPlayerController();
    virtual void BeginPlay() override;

	void SetInputByLevelName();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UGameSetWidget> GameSetWidgetClass;

    UPROPERTY()
    UGameSetWidget* GameSetWidget;

    UFUNCTION(Client, Reliable)
    void Client_ShowGameResult(const FString& ResultText);
};
