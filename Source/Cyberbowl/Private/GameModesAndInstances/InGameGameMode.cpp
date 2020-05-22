// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModesAndInstances/InGameGameMode.h"
#include "PlayerController/ThirdPersonPlayerController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Containers/Array.h"
#include <string>
#include "Components/TextBlock.h"
#include "Stadium/Goal_Collider.h"

void AInGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	PointsTeam0 = 0;
	PointsTeam1 = 0;

	TArray<AActor*> outGoals;
	UGameplayStatics::GetAllActorsOfClass(this, AGoal_Collider::StaticClass(), outGoals);

	for (auto actor : outGoals)
	{
		Cast<AGoal_Collider>(actor)->OnGoalScored.AddDynamic(this, &AInGameGameMode::Add_Points);
	}


	GetWorldTimerManager().SetTimer(GameEndTimerHandle, this, &AInGameGameMode::GameEnd, GamePlayTime);
	//Pause Game Timer til the countdown when starting the game ends.
	GetWorldTimerManager().PauseTimer(GameEndTimerHandle);
	GetWorldTimerManager().SetTimer(GameCountdownTimerHandle, this, &AInGameGameMode::Start, GameIntermediateTime);

	TArray<AActor*> controllers;
	UGameplayStatics::GetAllActorsOfClass(this, AThirdPersonPlayerController::StaticClass(), controllers);

	for (auto controller : controllers)
	{
		auto playerController = Cast<AThirdPersonPlayerController>(controller);

		playerController->OnPlayerPausedGame.AddDynamic(this, &AInGameGameMode::TogglePauseGame);
		playerControllers.AddUnique(playerController);
	}

	bGamePlayStarted = false;
	bLastMinuteFired = false;
}

void AInGameGameMode::GameEnd()
{
	TArray<AActor*> team0;
	TArray<AActor*> team1;
	
	if (PointsTeam0 > PointsTeam1)
	{
		WinningTeam = 0;
	}

	else if (PointsTeam0 < PointsTeam1)
	{
		WinningTeam = 1;
	}

	else
	{
		WinningTeam = 2;
	}
	EndGame.Broadcast();
	Ball->StopBall();
}

void AInGameGameMode::TogglePauseGame(int playerIndexInitiator)
{
	if (playerIndexInitiator != 0)
	{
		return;
	}
	
	if (bGameIsPaused)
	{
		ResumeGameForAll();
	}
	else
	{
		PauseGameForAll(playerIndexInitiator);
	}
}

void AInGameGameMode::PauseGameForAll(int playerIndexInitiator)
{
	UGameplayStatics::SetGlobalTimeDilation(this, 0);

	const auto controller = UGameplayStatics::GetPlayerControllerFromID(this, 0);

	pauseWidget = CreateWidget(controller, WGamePausedInitiator);
	pauseWidget->AddToViewport();
	Cast<UButton>(pauseWidget->GetWidgetFromName("Button_Resume"))->SetKeyboardFocus();

	bGameIsPaused = true;
}

void AInGameGameMode::ResumeGameForAll()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1);

	pauseWidget->RemoveFromParent();

	bGameIsPaused = false;
}

void AInGameGameMode::SetPauseWidget(UUserWidget* widget)
{
	pauseWidget = widget;
}

void AInGameGameMode::Add_Points(int teamIndex)
{
	if (teamIndex == 0)
	{
		PointsTeam1 += 1;
		ScoringTeam = 1;
		effectRotation = effectRotationTeam0;
	}
	else if (teamIndex == 1)
	{
		PointsTeam0 += 1;
		ScoringTeam = 0;
		effectRotation = effectRotationTeam1;
	}

	effectLocation = Ball->GetActorLocation();
	GetWorldTimerManager().PauseTimer(GameEndTimerHandle);
	PauseGamePlay.Broadcast();
	GetWorldTimerManager().SetTimer(GameIntermediateTimerHandle, this, &AInGameGameMode::RegroupPlayers, GameIntermediateTime);
	Ball->StopBall();
	Ball->HideBall(true);

}

void AInGameGameMode::Tick(float DeltaSeconds)
{
	GameEndTimeRemaining = GetWorldTimerManager().GetTimerRemaining(GameEndTimerHandle);
	GameIntermediateTimeRemaining = GetWorldTimerManager().GetTimerRemaining(GameCountdownTimerHandle);
	if(GameEndTimeRemaining<=60 && !bLastMinuteFired)
	{
		StartingLastMinute.Broadcast();
		bLastMinuteFired = true;
	}
}

void AInGameGameMode::SelectGameOverMenu(int LevelIndex)
{
	if (LevelIndex == 1)
	{
		UGameplayStatics::OpenLevel(this, "MainMenuMap");
	}

	else
	{
		UGameplayStatics::OpenLevel(this, "CyberbowlArenaMap");
	}
}

void AInGameGameMode::RegroupPlayers()
{
	Regroup.Broadcast();
	GetWorldTimerManager().SetTimer(GameCountdownTimerHandle, this, &AInGameGameMode::Start, GameIntermediateTime);
	Ball->ResetBallPosition();
	Ball->HideBall(false);
}

void AInGameGameMode::Start()
{
	if (!bGamePlayStarted)
	{
		MatchCoundownEnd.Broadcast();
		bGamePlayStarted = true;
	}
	StartGamePlay.Broadcast();
	RoundCoundownEnd.Broadcast();
	GetWorldTimerManager().UnPauseTimer(GameEndTimerHandle);
	Ball->PlayBall();
}

bool AInGameGameMode::GetIsPaused() const
{
	return bGameIsPaused;
}
