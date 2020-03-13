// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AirAbility.h"
#include "Kismet/KismetSystemLibrary.h"

void UAirAbility::Fire()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Air)Tornado-Thingy: ")));
}
