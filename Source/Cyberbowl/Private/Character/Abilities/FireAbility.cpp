// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/FireAbility.h"
#include "Kismet/KismetSystemLibrary.h"

void UFireAbility::Fire()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Fire)Wall-Thingy: ")));
}

void UFireAbility::Targeting()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Fire)Wall-Thingy: ")));
}

