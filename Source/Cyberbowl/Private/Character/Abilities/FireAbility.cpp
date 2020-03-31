// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/FireAbility.h"
#include "Kismet/KismetSystemLibrary.h"

void UFireAbility::BeginPlay()
{
	Super::BeginPlay();
	character = Cast<ACyberbowlCharacter>(GetOwner());
}


void UFireAbility::Fire()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Fire)Wall-Thingy: ")));


}

void UFireAbility::Targeting()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Fire)Wall-Thingy: ")));
}

void UFireAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrState == EAbilityState::ABILITY_FIRE)
	{
		UE_LOG(LogTemp, Warning, TEXT("FIRE FIRE!"));
	}
}

