// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCalculationActorComponent.h"
#include "SRCharacter.h"
// Sets default values for this component's properties
UCombatCalculationActorComponent::UCombatCalculationActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}





float UCombatCalculationActorComponent::DamageCalculate(class AController* EventInstigator, class AController* OwnerController)
{
	OtherCharacter = Cast<ASRCharacter>(EventInstigator->GetOwner());
	OwnerCharacter = Cast<ASRCharacter>(GetOwner());

	return 0.f;
}

void UCombatCalculationActorComponent::GaurdSuccess()
{
	
}

void UCombatCalculationActorComponent::ParrySuccess()
{
}


// Called when the game starts
void UCombatCalculationActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatCalculationActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

