// Fill out your copyright notice in the Description page of Project Settings.


#include "Points.h"

void APoints::BeginPlay()
{
	Super::BeginPlay();
	
}

void APoints::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UE_LOG(LogTemp, Warning, TEXT("Function called!cccccclöhkkjvfjfzözjc"));

	if (ball) {

		if (goal_collider_team0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Goal1 ready and called at every tick!"));
			

		}

		if (goal_collider_team1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Everything ready and called at every tick!"));
		}
	}
}