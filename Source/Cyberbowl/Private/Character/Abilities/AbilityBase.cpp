// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AbilityBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UAbilityBase::UAbilityBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAbilityBase::Fire()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Base)Fired ability: ")));
}

void UAbilityBase::Targeting()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Base)Targeting ability: ")));
}


// Called every frame
void UAbilityBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

