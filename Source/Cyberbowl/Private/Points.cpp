// Fill out your copyright notice in the Description page of Project Settings.


#include "Points.h"

void APoints::BeginPlay()
{
	Super::BeginPlay();
	PointsTeam0 = 0;
	PointsTeam1 = 0;
	
	GetWorldTimerManager().SetTimer(GameEndTimerHandle, this, &APoints::GameEnd, GamePlayTime);

}

void APoints::GameEnd()
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


void APoints::Add_Points(AActor* Collider)
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
	GetWorldTimerManager().SetTimer(GameIntermediateTimerHandle, this, &APoints::RegroupPlayers, GameIntermediateTime);
	
}

void APoints::Tick(float DeltaSeconds)
{
	GameEndTime = GetWorldTimerManager().GetTimerRemaining(GameEndTimerHandle);
}

void APoints::SelectGameOverMenu(int LevelIndex)
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

void APoints::RegroupPlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("Regrouping!!!!"));
	Regroup.Broadcast();
	GetWorldTimerManager().SetTimer(GameCountdownTimerHandle, this, &APoints::Restart, GameIntermediateTime);
}

void APoints::Restart()
{
	StartGamePlay.Broadcast();
	GetWorldTimerManager().UnPauseTimer(GameEndTimerHandle);
}