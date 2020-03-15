// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPlayerInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECBCharacterType : uint8
{
	CBCHRACTERTYPE_FIRE UMETA(DisplayName = "Fire"),
	CBCHRACTERTYPE_EARTH UMETA(DisplayName = "Earth"),
	CBCHRACTERTYPE_ICE UMETA(DisplayName = "Ice"),
	CBCHRACTERTYPE_AIR UMETA(DisplayName = "Air"),
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		int Team;
	UPROPERTY(BlueprintReadWrite)
		ECBCharacterType CharacterType;

	FPlayerInfo()
	{
		Team = 0;
		CharacterType = ECBCharacterType::CBCHRACTERTYPE_FIRE;
	}
};

UCLASS()
class CYBERBOWL_API UFPlayerInfo : public UObject
{
	GENERATED_BODY()
	
};
