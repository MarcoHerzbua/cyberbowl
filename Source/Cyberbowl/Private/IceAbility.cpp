// Fill out your copyright notice in the Description page of Project Settings.


#include "IceAbility.h"
#include "Kismet/KismetSystemLibrary.h"

void UIceAbility::Fire()
{
	UKismetSystemLibrary::PrintString(this, ((FString)(L"(Ice)Freeze-Thingy: ")));
}
