// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModesAndInstances/InGameGameMode.h"
#include <string>

void AInGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	PointsTeam0 = 0;
	PointsTeam1 = 0;


	GetWorldTimerManager().SetTimer(GameEndTimerHandle, this, &AInGameGameMode::GameEnd, GamePlayTime);
	StartGamePlay.Broadcast();

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

void AInGameGameMode::Add_Points(AActor* Collider)
{
	if (Collider->GetName()=="BP_Goal_Collider_Team0")
	{
		PointsTeam1 += 1;
		ScoringTeam = 1;
	}
	else if (Collider->GetName() == "BP_Goal_Collider_Team1")
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