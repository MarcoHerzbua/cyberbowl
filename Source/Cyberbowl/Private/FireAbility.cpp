// Fill out your copyright notice in the Description page of Project Settings.


#include "FireAbility.h"
#include "Kismet/KismetSystemLibrary.h"

void UFireAbility::Fire()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Fire)Wall-Thingy: ")));
}
