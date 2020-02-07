// Fill out your copyright notice in the Description page of Project Settings.


#include "Points.h"

void APoints::BeginPlay()
{
	Super::BeginPlay();
	PointsTeam0 = 0;
	PointsTeam1 = 0;
	float Game_Play_Time = 180.f;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APoints::GameEnd, Game_Play_Time);
	
}

void APoints::GameEnd()
{
	TArray<AActor*> team0;
	TArray<AActor*> team1;
	/*
	for (FCyberbowl_Characters Character : Characters)
	{
		if (Character.Team == 0)
			team0.Add(Character.Character);
		else
			team1.Add(Character.Character);
	} */
	
	if (PointsTeam0 > PointsTeam1)
	{
		WinningTeam = 0;
		//WinningTeam.Broadcast(0, team0);
	}

	else if (PointsTeam0 < PointsTeam1)
	{
		WinningTeam = 1;
		//WinningTeam.Broadcast(1, team1);
	}

	else
	{
		WinningTeam = 2;
		//WinningTeam.Broadcast(-1, team0);
	}
	EndGame.Broadcast();
}


void APoints::Add_Points(AActor* Collider)
{
	if (Collider->GetName()=="BP_Goal_Collider_Team0")
	{
		PointsTeam1 += 1;
		UE_LOG(LogTemp, Warning, TEXT("Collider 0"))
	}
	else if (Collider->GetName() == "BP_Goal_Collider_Team1")
	{
		PointsTeam0 += 1;
		UE_LOG(LogTemp, Warning, TEXT("Collider 1"));
	}
		
}

void APoints::Tick(float DeltaSeconds)
{
	Time = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
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
