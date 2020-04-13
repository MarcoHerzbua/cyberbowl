// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PlayBall.h"
#include "GameFramework/GameModeBase.h"
#include "TutorialGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CYBERBOWL_API ATutorialGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	APlayBall* Ball;
	UPROPERTY(BlueprintReadWrite)
	AActor* GoalColliderTeam0;
	UPROPERTY(BlueprintReadWrite)
	AActor* GoalColliderTeam1;
};
