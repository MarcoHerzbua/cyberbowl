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

USTRUCT(BlueprintType)
struct FCyberbowlCharacters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Team;
	
};


UCLASS()
class CYBERBOWL_API APoints : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Ball;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* GoalColliderTeam0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* GoalColliderTeam1;
	void Add_Points(AActor* Collider);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PointsTeam0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PointsTeam1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCyberbowlCharacters> Characters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WinningTeam;
	UFUNCTION(BlueprintCallable)
	void AddToCharacters(FCyberbowlCharacters character) { Characters.Add(character); }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;
	FTimerHandle TimerHandle;
	UPROPERTY(BlueprintAssignable)
	FEndGameEvent EndGame;

	UFUNCTION(BlueprintCallable)
	void SelectGameOverMenu(int LevelIndex);

private:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void GameEnd();
	
};
