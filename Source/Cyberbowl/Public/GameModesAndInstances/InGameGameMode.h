// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "InGameGameMode.generated.h"

class AThirdPersonPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndGameEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseGamePlayEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRegroupEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRestartGamePlayEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundCountdownEndingEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchountdownEndingEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartingLastMinuteEvent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CYBERBOWL_API AInGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	AActor* Ball;
	UPROPERTY( BlueprintReadWrite)
	AActor* GoalColliderTeam0;
	UPROPERTY(BlueprintReadWrite)
	AActor* GoalColliderTeam1;

	UFUNCTION()
	void Add_Points(int teamIndex);
	
	UPROPERTY(BlueprintReadWrite)
	int PointsTeam0;
	UPROPERTY(BlueprintReadWrite)
	int PointsTeam1;
	UPROPERTY(BlueprintReadWrite)
	int WinningTeam;
	UPROPERTY(BlueprintReadWrite)
	int ScoringTeam;
	UPROPERTY(BlueprintReadOnly)
	FVector effectLocation;
	UPROPERTY(BlueprintReadOnly)
	FRotator effectRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FRotator effectRotationTeam0 = FRotator(0,0,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator effectRotationTeam1 = FRotator(0, 180, 0);
	
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
	UPROPERTY(BlueprintAssignable)
	FRoundCountdownEndingEvent RoundCoundownEnd;
	UPROPERTY(BlueprintAssignable)
	FMatchountdownEndingEvent MatchCoundownEnd;
	UPROPERTY(BlueprintAssignable)
	FStartingLastMinuteEvent StartingLastMinute;

	UFUNCTION(BlueprintCallable)
	void SelectGameOverMenu(int LevelIndex);
	UFUNCTION(BlueprintCallable)
	void RegroupPlayers();
	UFUNCTION(BlueprintCallable)
	void Start();

	UFUNCTION()
	bool GetIsPaused() const;

private:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void GameEnd();

	UFUNCTION()
	void TogglePauseGame(int playerIndexInitiator);
	
	UFUNCTION()
	void PauseGameForAll(int playerIndexInitiator);

	UFUNCTION(BlueprintCallable)
	void ResumeGameForAll();

	UPROPERTY()
	bool bGameIsPaused;

	UPROPERTY()
	TArray<AThirdPersonPlayerController*> playerControllers;

	UPROPERTY()
	UUserWidget* pauseWidget;

	UFUNCTION(BlueprintCallable)
	void SetPauseWidget(UUserWidget* widget);

	UPROPERTY(Editanywhere)
	TSubclassOf<UUserWidget> WGamePausedInitiator;

	bool bGamePlayStarted;
	bool bLastMinuteFired;
};
