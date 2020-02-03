// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "Points.generated.h"


/**
 * 
 */
UCLASS()
class CYBERBOWL_API APoints : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* ball;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* goal_collider_team0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* goal_collider_team1;

	AActor* GetBall() { return ball; };

private:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
};
