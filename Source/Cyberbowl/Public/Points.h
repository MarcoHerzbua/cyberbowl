// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "../CyberbowlCharacter.h"
#include "Points.generated.h"


/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndGameEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseGamePlayEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRegroupEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRestartGamePlayEvent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CYBERBOWL_API APoints : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	AActor* Ball;
	UPROPERTY( BlueprintReadWrite)
	AActor* GoalColliderTeam0;
	UPROPERTY(BlueprintReadWrite)
	AActor* GoalColliderTeam1;
	void Add_Points(AActor* Collider);
	UPROPERTY(BlueprintReadWrite)
	int PointsTeam0;
	UPROPERTY(BlueprintReadWrite)
	int PointsTeam1;
	UPROPERTY(BlueprintReadWrite)
	int WinningTeam;
	UPROPERTY(BlueprintReadWrite)
	int ScoringTeam;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GamePlayTime = 180.f;
	UPROPERTY(BlueprintReadWrite)
	float GameEndTimeRemaining;
	FTimerHandle GameEndTimerHandle;
	UPROPERTY(BlueprintAssignable)
	FEndGameEvent EndGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameIntermediateTime = 3.f;
	FTimerHandle GameIntermediateTimerHandle;
	FTimerHandle GameCountdownTimerHandle;
	UPROPERTY(BlueprintReadWrite)
	float GameIntermediateTimeRemaining;
	UPROPERTY(BlueprintAssignable)
	FPauseGamePlayEvent PauseGamePlay;
	UPROPERTY(BlueprintAssignable)
	FRegroupEvent Regroup;
	UPROPERTY(BlueprintAssignable)
	FRestartGamePlayEvent StartGamePlay;

	UFUNCTION(BlueprintCallable)
	void SelectGameOverMenu(int LevelIndex);
	UFUNCTION(BlueprintCallable)
	void RegroupPlayers();
	UFUNCTION(BlueprintCallable)
	void Restart();

private:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void GameEnd();
	
};
