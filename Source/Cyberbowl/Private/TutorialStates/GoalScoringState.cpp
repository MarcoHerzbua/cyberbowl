// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialStates/GoalScoringState.h"


#include "GameModesAndInstances/TutorialGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGoalScoringState::EnterState()
{
	UKismetSystemLibrary::PrintString(this, "Entered GoalScoringState");
}

void UGoalScoringState::UpdateState(float deltaSeconds)
{
	stateTimePassed += deltaSeconds;
	if (stateTimePassed > 3.f)
	{
		Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(this))->AdvanceTutorial();
	}
}

void UGoalScoringState::ExitState()
{
	UKismetSystemLibrary::PrintString(this, "Exited GoalScoringState");
}
