// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CyberbowlGameInstance.generated.h"


//UENUM(BlueprintType)
//enum class EPlayerCharacterType : uint8
//{
//    Fire 	UMETA(DisplayName = "Fire"),
//    Earth 	UMETA(DisplayName = "Earth"),
//    Water	UMETA(DisplayName = "Water"),
//    Air     UMETA(DisplayName = "Air")
//};
//
//USTRUCT(BlueprintType)
//struct FLobbyPlayerInfo
//{
//    GENERATED_BODY()
//
//    UPROPERTY(EditAnywhere)
//    int Team;
//
//    UPROPERTY(EditAnywhere)
//    EPlayerCharacterType CharacterType;
//};

/**
 * 
 */
UCLASS()
class CYBERBOWL_API UCyberbowlGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

//public:
//    UPROPERTY(EditAnywhere)
//    TMap<int, FLobbyPlayerInfo> PlayerInfoMap;

};
