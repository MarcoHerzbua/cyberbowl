// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WNameTag.generated.h"


UCLASS()
class CYBERBOWL_API UWNameTag : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	int PlayerIndex;
	
	UPROPERTY(BlueprintReadOnly)
	FString CharacterName = "Default";

	UPROPERTY(BlueprintReadWrite)
	FLinearColor ColorTeam1;
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor ColorTeam2;

};
