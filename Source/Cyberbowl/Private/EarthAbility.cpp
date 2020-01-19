// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthAbility.h"
#include "Kismet/KismetSystemLibrary.h"

void UEarthAbility::Fire()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Earth)Pillar-Thingy: ")));
}
