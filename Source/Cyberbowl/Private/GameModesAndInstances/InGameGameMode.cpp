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
	StartGamePlay.Broadcast();

	TArray<AActor*> controllers;
	UGameplayStatics::GetAllActorsOfClass(this, AThirdPersonPlayerController::StaticClass(), controllers);

	for (auto controller : controllers)
	{
		auto playerController = Cast<AThirdPersonPlayerController>(controller);

		playerController->OnPlayerPausedGame.AddDynamic(this, &AInGameGameMode::TogglePauseGame);
		playerControllers.AddUnique(playerController);
	}
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
}

void AInGameGameMode::TogglePauseGame(int playerIndexInitiator)
{
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
	
	for (auto playerController : playerControllers)
	{		
		if (UGameplayStatics::GetPlayerControllerID(playerController) == playerIndexInitiator)
		{
			auto widget = CreateWidget(playerController, WGamePausedInitiator);
			widget->AddToPlayerScreen();
			Cast<UButton>(widget->GetWidgetFromName("Button_Resume"))->SetKeyboardFocus();
			
			pauseWidgets.AddUnique(widget);
		}
		else
		{
			auto widget = CreateWidget(playerController, WGamePausedAll);
			const int indexToActualPlayer = playerIndexInitiator + 1;
			FString widgetText = FString("Player ").Append(std::to_string(indexToActualPlayer).c_str()).Append(" has paused the game!");
			Cast<UTextBlock>(widget->GetWidgetFromName("PauseGameInitiatorText"))->SetText(FText::FromString(widgetText));
			widget->AddToPlayerScreen();
			
			pauseWidgets.AddUnique(widget);
		}
	}

	bGameIsPaused = true;
}

void AInGameGameMode::ResumeGameForAll()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1);
	
	for (auto pauseWidget : pauseWidgets)
	{
		pauseWidget->RemoveFromParent();
	}
	
	pauseWidgets.Empty();

	bGameIsPaused = false;
}

void AInGameGameMode::Add_Points(int teamIndex)
{
	if (teamIndex == 0)
	{
		PointsTeam1 += 1;
		ScoringTeam = 1;
	}
	else if (teamIndex == 1)
	{
		PointsTeam0 += 1;
		ScoringTeam = 0;
	}

	GetWorldTimerManager().PauseTimer(GameEndTimerHandle);
	PauseGamePlay.Broadcast();
	GetWorldTimerManager().SetTimer(GameIntermediateTimerHandle, this, &AInGameGameMode::RegroupPlayers, GameIntermediateTime);
	
}

void AInGameGameMode::Tick(float DeltaSeconds)
{
	GameEndTimeRemaining = GetWorldTimerManager().GetTimerRemaining(GameEndTimerHandle);
	GameIntermediateTimeRemaining = GetWorldTimerManager().GetTimerRemaining(GameCountdownTimerHandle);
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
	GetWorldTimerManager().SetTimer(GameCountdownTimerHandle, this, &AInGameGameMode::Restart, GameIntermediateTime);

}

void AInGameGameMode::Restart()
{
	StartGamePlay.Broadcast();
	GetWorldTimerManager().UnPauseTimer(GameEndTimerHandle);
}

bool AInGameGameMode::GetIsPaused() const
{
	return bGameIsPaused;
}
